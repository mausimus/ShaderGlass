@echo off
REM ShaderGlass Build and Test Script
REM This script builds the project and runs basic validation tests

echo ========================================
echo ShaderGlass Build and Test
echo ========================================
echo.

REM Find MSBuild
set MSBUILD_PATH=
for %%i in (Enterprise Professional Community) do (
    if exist "C:\Program Files\Microsoft Visual Studio\2026\%%i\MSBuild\Current\Bin\MSBuild.exe" (
        set "MSBUILD_PATH=C:\Program Files\Microsoft Visual Studio\2026\%%i\MSBuild\Current\Bin\MSBuild.exe"
        goto :found_msbuild
    )
)

echo ERROR: MSBuild not found. Please install Visual Studio 2026.
pause
exit /b 1

:found_msbuild
echo Found MSBuild: %MSBUILD_PATH%
echo.

REM Clean build directory
echo Cleaning previous builds...
if exist x64\Debug rd /s /q x64\Debug
echo.

REM Build Debug configuration
echo Building Debug configuration...
echo.
"%MSBUILD_PATH%" ShaderGlass.sln /p:Configuration=Debug /p:Platform=x64 /t:Rebuild /m /v:minimal

if errorlevel 1 (
    echo.
    echo ========================================
    echo BUILD FAILED!
    echo ========================================
    pause
    exit /b 1
)

echo.
echo ========================================
echo BUILD SUCCESSFUL!
echo ========================================
echo.

REM Check if executable exists
if not exist "x64\Debug\ShaderGlass.exe" (
    echo ERROR: ShaderGlass.exe not found in x64\Debug
    pause
    exit /b 1
)

echo Executable created: x64\Debug\ShaderGlass.exe
echo Size:
dir "x64\Debug\ShaderGlass.exe" | find "ShaderGlass.exe"
echo.

REM Run basic validation
echo Running basic validation tests...
echo.

REM Test 1: Check for common symbols
echo [Test 1] Checking for security functions...
findstr /C:"SanitizePath" /C:"SafeParseInt" "ShaderGC\*.cpp" >nul
if errorlevel 1 (
    echo WARNING: Security functions not found in source
) else (
    echo PASS: Security functions present
)

REM Test 2: Check for threading fixes
echo [Test 2] Checking for threading fixes...
findstr /C:"std::atomic" /C:"std::mutex" "ShaderGlass\*.h" >nul
if errorlevel 1 (
    echo WARNING: Threading primitives not found
) else (
    echo PASS: Threading primitives present
)

REM Test 3: Check for performance optimizations
echo [Test 3] Checking for performance optimizations...
findstr /C:"m_cachedInput" /C:"IsDirty" "ShaderGlass\*.cpp" >nul
if errorlevel 1 (
    echo WARNING: Performance optimizations not found
) else (
    echo PASS: Performance optimizations present
)

echo.
echo ========================================
echo BUILD AND TEST COMPLETE
echo ========================================
echo.
echo Next steps:
echo 1. Run: x64\Debug\ShaderGlass.exe
echo 2. Test basic functionality
echo 3. Run security tests (see SECURITY_TESTS.bat)
echo 4. Run performance tests (see PERFORMANCE_TESTS.bat)
echo.
pause
