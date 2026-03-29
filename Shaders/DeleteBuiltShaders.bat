@echo off
echo This script will delete all prebuilt shaders from ShaderGlass.
echo,
pause
del /q ..\ShaderGlass\Shaders\RetroArch.h
rmdir /s /q ..\ShaderGlass\Shaders\RetroArch
rmdir /s /q temp
