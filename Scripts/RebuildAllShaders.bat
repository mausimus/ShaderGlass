@echo off
echo This script will rebuild *ALL* RetroArch shaders into ShaderGlass.
echo,
echo Make sure you have ShaderGen.exe built into x64\Debug directory
echo and RetroArch shaders cloned into Scripts\slang-shaders subdirectory.
echo,
echo This will take A LONG TIME (30 min+)
pause

del /q ..\ShaderGlass\Shaders\RetroArch.h
rmdir /s /q ..\ShaderGlass\Shaders\RetroArch
rmdir /s /q temp
..\x64\Release\ShaderGen.exe *
