@echo off
if [%1]==[] goto usage
..\x64\Release\ShaderGen.exe --force %1
goto :eof

:usage
echo This script will rebuild a single shader from RetroArch shaders into ShaderGlass.
echo,
echo Make sure you have ShaderGen.exe built into \x64\Release directory
echo and RetroArch shaders cloned into \Scripts\slang-shaders subdirectory.
echo,
echo Usage:  RebuildShader.bat ^<shader-path-under-slang-shaders^>
echo   i.e.  RebuildShader.bat pal\pal-r57shell.slangp^
