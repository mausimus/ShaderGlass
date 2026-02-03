@echo off
REM Manual Build Script - Searches more locations for MSBuild

echo Searching for MSBuild installation...
echo.

REM Try to find MSBuild using vswhere (recommended method)
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"

if exist "%VSWHERE%" (
    echo Found vswhere.exe - using it to locate MSBuild...
    for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\MSBuild.exe`) do (
        set MSBUILD_PATH=%%i
        goto :found_msbuild
    )
)

REM Manual search in common locations
echo Searching common Visual Studio locations...
echo.

REM VS 2026 locations
for %%V in (Enterprise Professional Community BuildTools) do (
    if exist "C:\Program Files\Microsoft Visual Studio\2026\%%V\MSBuild\Current\Bin\MSBuild.exe" (
        set "MSBUILD_PATH=C:\Program Files\Microsoft Visual Studio\2026\%%V\MSBuild\Current\Bin\MSBuild.exe"
        goto :found_msbuild
    )
    if exist "C:\Program Files (x86)\Microsoft Visual Studio\2026\%%V\MSBuild\Current\Bin\MSBuild.exe" (
        set "MSBUILD_PATH=C:\Program Files (x86)\Microsoft Visual Studio\2026\%%V\MSBuild\Current\Bin\MSBuild.exe"
        goto :found_msbuild
    )
)

REM VS 2022 locations (fallback)
for %%V in (Enterprise Professional Community BuildTools) do (
    if exist "C:\Program Files (x86)\Microsoft Visual Studio\2022\%%V\MSBuild\Current\Bin\MSBuild.exe" (
        set "MSBUILD_PATH=C:\Program Files (x86)\Microsoft Visual Studio\2022\%%V\MSBuild\Current\Bin\MSBuild.exe"
        echo WARNING: Found Visual Studio 2022 - ShaderGlass requires VS 2026
        echo You may encounter compatibility issues.
        goto :found_msbuild
    )
)

REM Try .NET SDK location (if dotnet SDK installed)
for /f "delims=" %%i in ('where dotnet 2^>nul') do (
    for /f "delims=" %%j in ('dotnet --list-sdks 2^>nul') do (
        echo Found .NET SDK but need Visual Studio Build Tools
    )
)

echo.
echo ========================================
echo MSBuild NOT FOUND
echo ========================================
echo.
echo ShaderGlass requires Visual Studio 2026 or Build Tools.
echo.
echo OPTIONS:
echo.
echo 1. Install Build Tools (Free, 7 GB)
echo    https://visualstudio.microsoft.com/downloads/
echo    Select: "Build Tools for Visual Studio 2026"
echo.
echo 2. Install Visual Studio Community (Free, 15 GB)
echo    https://visualstudio.microsoft.com/vs/community/
echo    Select workload: "Desktop development with C++"
echo.
echo 3. Manually specify MSBuild path
echo    Edit this script and set MSBUILD_PATH at top
echo.
echo For detailed instructions, see: INSTALL_BUILD_TOOLS.md
echo.
pause
exit /b 1

:found_msbuild
echo ========================================
echo MSBuild Found!
echo ========================================
echo Location: %MSBUILD_PATH%
echo.

REM Display version
"%MSBUILD_PATH%" /version | findstr /C:"Microsoft"
echo.

REM Ask user if they want to proceed
set /p "PROCEED=Build ShaderGlass now? (Y/N): "
if /i not "%PROCEED%"=="Y" (
    echo Build cancelled.
    pause
    exit /b 0
)

echo.
echo ========================================
echo Building ShaderGlass (Debug x64)
echo ========================================
echo.

REM Build the solution
"%MSBUILD_PATH%" ShaderGlass.sln /p:Configuration=Debug /p:Platform=x64 /t:Rebuild /m /v:minimal /nologo

if errorlevel 1 (
    echo.
    echo ========================================
    echo BUILD FAILED!
    echo ========================================
    echo.
    echo Check the output above for errors.
    echo Common issues:
    echo - Missing Windows SDK
    echo - Missing C++ build tools
    echo - Syntax errors in code
    echo.
    pause
    exit /b 1
)

echo.
echo ========================================
echo BUILD SUCCESSFUL!
echo ========================================
echo.

if exist "x64\Debug\ShaderGlass.exe" (
    echo Executable: x64\Debug\ShaderGlass.exe
    dir "x64\Debug\ShaderGlass.exe" | find "ShaderGlass.exe"
    echo.
    echo Ready to test! Run:
    echo   x64\Debug\ShaderGlass.exe
    echo.
) else (
    echo WARNING: Build succeeded but executable not found
    echo Expected location: x64\Debug\ShaderGlass.exe
    echo.
)

pause
