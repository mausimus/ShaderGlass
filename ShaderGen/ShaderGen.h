/*
ShaderGen: shader precompiler for ShaderGlass
Copyright (C) 2021-2025 mausimus (mausimus.net)
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0
*/

#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

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

// RAII wrapper for temporary files with secure cleanup
class TempFile
{
private:
    filesystem::path m_path;
    bool m_created;

public:
    TempFile(const filesystem::path& basePath, const string& prefix, const string& extension)
        : m_created(false)
    {
        // Create unique temporary file name using current process ID and counter
        static std::atomic<int> counter{0};
        int id = counter.fetch_add(1);

        stringstream filename;
        filename << prefix << "_" << GetCurrentProcessId() << "_" << id << extension;

        m_path = basePath / filename.str();
        m_created = false;
    }

    const filesystem::path& path() const { return m_path; }

    void markCreated() { m_created = true; }

    ~TempFile()
    {
        // Securely delete temporary file on destruction
        if(m_created && filesystem::exists(m_path))
        {
            try
            {
                filesystem::remove(m_path);
            }
            catch(...)
            {
                // Best effort cleanup - log but don't throw from destructor
            }
        }
    }

    // No copying or assignment
    TempFile(const TempFile&) = delete;
    TempFile& operator=(const TempFile&) = delete;
};

// paths relative to starting in Scripts directory
const char* _libName      = "RetroArch";
const char* _inputPath    = "slang-shaders";
const char* _templatePath = "..\\ShaderGen\\";
const char* _toolsPath    = "..\\Tools\\";
const char* _outputPath   = "..\\ShaderGlass\\Shaders\\";
const char* _tempPath     = "temp";

// only needed when using -tools option
const char* _fxcPath  = "C:\\Program Files (x86)\\Windows Kits\\10\\bin\\10.0.26100.0\\x64\\fxc.exe";
const char* _glslExe  = "glslangValidator.exe";
const char* _spirvExe = "spirv-cross.exe";

const char*      _raUrl = "https://github.com/libretro/slang-shaders/blob/f1796f6f744c32da57b9d8c27ea1a20160128696/";
const char*      _mbUrl = "https://github.com/libretro/slang-shaders/blob/f1796f6f744c32da57b9d8c27ea1a20160128696/bezel/Mega_Bezel";
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
