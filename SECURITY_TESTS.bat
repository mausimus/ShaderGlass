@echo off
REM Security Testing Script for ShaderGlass
REM Tests all security fixes with malicious inputs

echo ========================================
echo ShaderGlass Security Tests
echo ========================================
echo.

setlocal enabledelayedexpansion

set PASS_COUNT=0
set FAIL_COUNT=0
set TOTAL_TESTS=0

REM Create test directory
if not exist "SecurityTests" mkdir SecurityTests
cd SecurityTests

echo Creating malicious test files...
echo.

REM ========================================
REM Test 1: Path Traversal Attack
REM ========================================
set /a TOTAL_TESTS+=1
echo [Test 1] Path Traversal - Directory traversal attempt
echo Creating malicious shader path: ..\..\..\Windows\System32\cmd.exe

REM Create a fake shader file with traversal path
echo #version 450 > "..\..\malicious.slangp"
echo // This should be blocked > "..\..\malicious.slangp"

echo Expected: Path should be sanitized and blocked
echo Action: Try importing shader with path: ..\..\malicious.slangp
echo Manual test required - check if path is rejected
echo.
set /a PASS_COUNT+=1

REM ========================================
REM Test 2: Command Injection
REM ========================================
set /a TOTAL_TESTS+=1
echo [Test 2] Command Injection - Shell metacharacters in path
echo Creating shader with dangerous filename

REM Create files with shell metacharacters
echo test > "shader^&calc.slangp"
echo test > "shader;notepad.slangp"
echo test > "shader`whoami`.slangp"

echo Expected: Special characters should be escaped
echo Action: Try importing these shaders
echo Manual test required - verify no command execution
echo.
set /a PASS_COUNT+=1

REM ========================================
REM Test 3: Integer Overflow
REM ========================================
set /a TOTAL_TESTS+=1
echo [Test 3] Integer Overflow - Extremely large dimensions
echo Creating shader with overflow values

echo #reference overflow_test > overflow_test.slang
echo layout(std140, set = 0, binding = 0) uniform UBO { >> overflow_test.slang
echo     vec4 SourceSize;  // Try setting to 9999999999 >> overflow_test.slang
echo }; >> overflow_test.slang

echo Expected: Large values should be rejected or clamped
echo Action: Try setting texture size to 9999999999
echo Manual test required - check for overflow protection
echo.
set /a PASS_COUNT+=1

REM ========================================
REM Test 4: Resource Exhaustion (DoS)
REM ========================================
set /a TOTAL_TESTS+=1
echo [Test 4] Resource Exhaustion - Excessive shader passes
echo Creating shader with many passes

echo shaders = "100" > dos_test.slangp
echo shader0 = "test.slang" >> dos_test.slangp
for /L %%i in (1,1,99) do (
    echo shader%%i = "test.slang" >> dos_test.slangp
)

echo Expected: Number of passes should be limited to MAX_SHADER_PASSES
echo Action: Try loading shader with 100 passes
echo Manual test required - verify limit is enforced
echo.
set /a PASS_COUNT+=1

REM ========================================
REM Test 5: Temp File Security
REM ========================================
set /a TOTAL_TESTS+=1
echo [Test 5] Temp File Security - Predictable temp file names

echo Expected: Temp files should use unique, unpredictable names
echo Action: Check temp file creation in ShaderGen
echo Look for GetTempFileName usage instead of hardcoded paths
echo Manual test required - inspect temp file names
echo.
set /a PASS_COUNT+=1

REM ========================================
REM Test 6: Buffer Overflow
REM ========================================
set /a TOTAL_TESTS+=1
echo [Test 6] Buffer Overflow - Extremely long strings

REM Create a file with very long line
set "LONGSTRING="
for /L %%i in (1,1,100) do set "LONGSTRING=!LONGSTRING!AAAAAAAAAA"

echo !LONGSTRING! > long_line.slangp

echo Expected: Long lines should be truncated or rejected
echo Action: Try loading shader with 1000+ character lines
echo Manual test required - verify MAX_LINE_LENGTH enforcement
echo.
set /a PASS_COUNT+=1

REM ========================================
REM Summary
REM ========================================
echo.
echo ========================================
echo Security Test Summary
echo ========================================
echo Total Tests: %TOTAL_TESTS%
echo Tests Created: %PASS_COUNT%
echo.
echo All test files created in: %CD%
echo.
echo IMPORTANT: These are MANUAL tests
echo Run ShaderGlass and attempt to:
echo 1. Import each malicious shader file
echo 2. Verify the attack is blocked
echo 3. Check for error messages
echo 4. Ensure no crashes or hangs
echo.
echo Expected Behavior:
echo - Path traversal: Rejected with sanitization
echo - Command injection: Special chars escaped
echo - Integer overflow: Values clamped/rejected
echo - Resource exhaustion: Limited to 64 passes
echo - Temp files: Unique names with GetTempFileName
echo - Buffer overflow: Lines truncated to 4096 chars
echo.

cd ..

echo Detailed security validation checklist:
echo.
echo [ ] Test 1: Path traversal blocked
echo [ ] Test 2: Command injection prevented
echo [ ] Test 3: Integer overflow protected
echo [ ] Test 4: DoS limits enforced
echo [ ] Test 5: Temp file security verified
echo [ ] Test 6: Buffer overflow prevented
echo.
echo Document results in: SecurityTests\RESULTS.txt
echo.
pause
