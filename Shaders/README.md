# Rebuilding Shaders

ShaderGlass comes with a precompiled library of shaders embedded in the binary. If you'd like to add
or modify an embedded shader, you will need to rebuild it and then recompile ShaderGlass.

To try out an external shader, you can simply "Import custom..." which will load it on-the-fly.

ShaderGen is a tool that reads .slangp profiles and generates header files
which are then compiled with ShaderGlass.

## Rebuilding whole shader library

0. Build ShaderGen using Visual Studio in Release configuration
1. Clone slang-shaders submodule inside Shaders directory

> This will download RetroArch shaders from ShaderGlass' fork, which includes
tweaks and exclusions. You can clone original slang-shaders repo instead
but beware many shaders will fail to compile or not work properly.

2. Run RebuildAllShaders.bat
> This will try to rebuild all shaders (takes a bit of time, 10 mins or so)

3. Rebuild ShaderGlass using Visual Studio

## Rebuilding a single shader

Instead of rebuilding all shaders you can focus on a single .slangp shader.

1. Go into Scripts folder
2. Run RebuildShader.bat [shader-path] (see .bat for usage)
3. Rebuild ShaderGlass using Visual Studio

## Debugging

ShaderGen will generate log and intermediate files in temp subdirectory.
You can check there for compilation errors/warnings.
