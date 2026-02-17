/*
ShaderGlass: shader effect overlay
Copyright (C) 2021-2025 mausimus (mausimus.net)
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0
*/

#include "pch.h"

#pragma once

#include "ShaderCache.h"
#include "ShaderList.h"

#include "shaders\RetroArch.h"

std::vector<PresetDef*> RetroArchPresetList = RetroArch::PresetList;

std::vector<CachedShader> RetroArchCachedShaders()
{
    return RetroArch::CachedShaders();
}