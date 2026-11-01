/*
ShaderGen: shader precompiler for ShaderGlass
Copyright (C) 2021-2025 mausimus (mausimus.net)
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0
*/

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <algorithm>
#include <iomanip>
#include <cctype>
#include <locale>
#include <map>
#include <unordered_set>
#include <filesystem>

#include "SourceDefs.h"

using namespace std;

// paths relative to starting in Shaders directory
const char* _libName      = "RetroArch";
const char* _inputPath    = "slang-shaders";
const char* _templatePath = "..\\ShaderGen\\";
const char* _toolsPath    = "..\\Dependencies\\bin\\";
const char* _outputPath   = "..\\ShaderGlass\\Shaders\\";
const char* _tempPath     = "temp";

// only needed when using -tools option
const char* _fxcPath  = "C:\\Program Files (x86)\\Windows Kits\\10\\bin\\10.0.26100.0\\x64\\fxc.exe";
const char* _glslExe  = "glslangValidator.exe";
const char* _spirvExe = "spirv-cross.exe";

const char*      _raUrl = "https://github.com/libretro/slang-shaders/blob/a4f3aeec04fcb2624ec6df5dd17e38f9b575eab9/";
const char*      _mbUrl = "https://github.com/libretro/slang-shaders/blob/a4f3aeec04fcb2624ec6df5dd17e38f9b575eab9/bezel/Mega_Bezel";
const char*      _skUrl = "https://forums.libretro.com/t/new-sonkun-crt-guest-advanced-hd-presets-thread/39091";
const char*      _rcUrl = "https://github.com/RetroCrisis/Retro-Crisis-GDV-NTSC";
bool             _force = false;
bool             _tools = false;
filesystem::path outputPath;

void replace(string& str, const string& macro, const string& value)
{
    auto i = str.find(macro);
    while(i != string::npos)
    {
        str.replace(i, macro.length(), value);
        i = str.find(macro);
    }
}

SourceShaderInfo getShaderInfo(const filesystem::path& slangInput, const string& suffix, bool fullPath = true)
{
    SourceShaderInfo info;

    if(!slangInput.has_parent_path())
        info.category = "stock";
    else if(fullPath)
    {
        info.category = slangInput.parent_path().string();
    }
    else
        info.category = slangInput.begin()->string();

    const auto&  stem = slangInput.filename().stem();
    stringstream className;
    stringstream shaderName;
    bool         upper = true;
    for(const auto& c : info.category)
    {
        if(c == '-' || c == '\\' || c == '.' || c == '-' || c == '+' || c == ' ' || c == '/' || c == '(' || c == ')')
        {
            upper = true;
            continue;
        }
        if(upper)
        {
            className << (char)toupper(c);
            upper = false;
        }
        else
            className << c;
    }
    upper = true;
    for(const auto& c : stem.string())
    {
        if(c == '-' || c == '.' || c == '-' || c == '+' || c == ' ' || c == '/' || c == '(' || c == ')')
        {
            upper = true;
            shaderName << c;
            continue;
        }
        if(upper)
        {
            className << (char)toupper(c);
            upper = false;
        }
        else
            className << c;

        shaderName << c;
    }
    info.className    = className.str();
    info.shaderName   = shaderName.str();
    info.sourcePath   = slangInput;
    info.relativePath = filesystem::path(string(_libName) + "\\" + info.category + "\\" + info.className + suffix + ".h").lexically_normal();
    info.outputPath   = filesystem::path(outputPath / info.relativePath.string()).lexically_normal();
    filesystem::create_directories(info.outputPath.parent_path());

    replace(info.category, "\\", "/");
    //replace(info.category, "/", "-");
    replace(info.category, " ", "-");
    replace(info.category, "+", "-");
    replace(info.category, "/Mega_Bezel/Presets", "");
    replace(info.category, "Base_CRT_Presets", "Mega_Bezel_Base");
    replace(info.category, "koko-aio/Presets-ng/TV", "koko-aio");
    replace(info.category, "koko-aio/Presets-ng/VGA", "koko-aio");
    replace(info.category, "koko-aio/Presets-ng/VectorGFX", "koko-aio");
    replace(info.category, "koko-aio/Presets-ng", "koko-aio");
    replace(info.category, "koko-aio/Presets_Handhelds-ng", "koko-aio");
    replace(info.category, "koko-aio/Presets_HiresGames_Fast/Presets-ng/TV", "koko-aio-HiresGames_Fast");
    replace(info.category, "koko-aio/Presets_HiresGames_Fast/Presets-ng/VGA", "koko-aio-HiresGames_Fast");
    replace(info.category, "koko-aio/Presets_HiresGames_Fast/Presets-ng/VectorGFX", "koko-aio-HiresGames_Fast");
    replace(info.category, "koko-aio/Presets_HiresGames_Fast/Presets-ng", "koko-aio-HiresGames_Fast");
    replace(info.category, "koko-aio/Presets_HiresGames_Fast/Presets_Handhelds-ng", "koko-aio-HiresGames_Fast");
    replace(info.category, "koko-aio/Presets_HiresGames_Fast", "koko-aio-HiresGames_Fast");
    replace(info.category, "koko-aio/Presets-ng", "koko-aio-ng");
    replace(info.shaderName, "1080p-crt-guest-advanced-hd-aperture-grille-", "");
    replace(info.shaderName, "1440p-crt-guest-advanced-hd-aperture-grille-", "");
    replace(info.shaderName, "4k-crt-guest-advanced-hd-aperture-grille-", "");
    replace(info.shaderName, "1080p-crt-guest-advanced-hd-shadow-mask-", "");
    replace(info.shaderName, "1440p-crt-guest-advanced-hd-shadow-mask-", "");
    replace(info.shaderName, "4k-crt-guest-advanced-hd-shadow-mask-", "");
    replace(info.shaderName, "1080p-crt-guest-advanced-hd-slot-mask-", "");
    replace(info.shaderName, "1440p-crt-guest-advanced-hd-slot-mask-", "");
    replace(info.shaderName, "4k-crt-guest-advanced-hd-slot-mask-", "");

    return info;
}
