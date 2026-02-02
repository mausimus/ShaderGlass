/*
ShaderGlass: Security limits for resource exhaustion protection
Copyright (C) 2021-2025 mausimus (mausimus.net)
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0
*/

#pragma once

#include <cstddef>

namespace SecurityLimits
{
    // Maximum shader source code size (10 MB)
    constexpr size_t MAX_SHADER_SOURCE_SIZE = 10 * 1024 * 1024;

    // Maximum compiled shader bytecode size (5 MB)
    constexpr size_t MAX_SHADER_BYTECODE_SIZE = 5 * 1024 * 1024;

    // Maximum texture dimensions (16K x 16K - reasonable for 2026)
    constexpr int MAX_TEXTURE_WIDTH = 16384;
    constexpr int MAX_TEXTURE_HEIGHT = 16384;

    // Maximum texture file size (500 MB for uncompressed 16K textures)
    constexpr size_t MAX_TEXTURE_FILE_SIZE = 500 * 1024 * 1024;

    // Maximum number of shader passes in a preset
    constexpr int MAX_SHADER_PASSES = 100;

    // Maximum number of textures in a preset
    constexpr int MAX_TEXTURES = 100;

    // Maximum number of parameters in a shader
    constexpr int MAX_PARAMETERS = 1000;

    // Maximum string length for names/paths
    constexpr size_t MAX_STRING_LENGTH = 4096;
}
