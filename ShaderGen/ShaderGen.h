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

#include "include/json.hpp"

using json = nlohmann::json;
using namespace std;

const char* _libName    = "RetroArch";
const char* _inputPath  = "..\\..\\slang-shaders";
const char* _outputPath = "..\\ShaderGlass\\ShaderGlass\\Shaders\\";
const char* _glslPath   = "..\\ShaderGlass\\Tools\\glslangValidator.exe";
const char* _spirvPath  = "..\\ShaderGlass\\Tools\\spirv-cross.exe";
const char* _tempPath   = "..\\ShaderGlass\\temp";
const char* _fxcPath    = "C:\\Program Files (x86)\\Windows Kits\\10\\bin\\10.0.22621.0\\x64\\fxc.exe";
const char* _raUrl      = "https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/";
bool _force = false;

static inline void ltrim(std::string& s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch) && ch != '\"'; }));
}

static inline void rtrim(std::string& s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch) && ch != '\"'; }).base(), s.end());
}

static inline string trim(std::string s)
{
    ltrim(s);
    rtrim(s);
    return s;
}

struct ShaderParam
{
    ShaderParam(string s, int size, int buffer) : i {-1}, buffer {buffer}, size {size}, offset {}, name {}, desc {}, min {}, max {}, def {}, step {}
    {
        istringstream iss(s);

        iss >> name;
        iss >> name;
        iss >> name;
        iss >> quoted(desc);
        desc = trim(desc);
        iss >> def;
        iss >> min;
        iss >> max;
        if(!iss.eof())
            iss >> step;
        else
            step = 0;
    }

    ShaderParam(string s, float o) : ShaderParam(s, 4, 0)
    {
        def = o;
    }

    int    buffer; // -1 - push constant, 0 - first UBO, etc.
    int    i;
    int    size;
    int    offset;
    string name;
    string desc;
    float  min;
    float  max;
    float  def;
    float  step;
};

struct ShaderSampler
{
    ShaderSampler(string name, int binding) : name {name}, binding {binding} { }
    string name;
    int    binding;
};

struct PresetTexture
{
    PresetTexture(string name) : name {name}, source {}, linear {}, wrap {}, mipmap {} { }
    string name;
    string source;
    bool   linear;
    bool   wrap;
    bool   mipmap;
};

struct ShaderInfo
{
    filesystem::path sourcePath;
    filesystem::path outputPath;
    filesystem::path relativePath;
    string           className;
    string           shaderName;
    string           category;
};

void replace(string& str, const string& macro, const string& value)
{
    auto i = str.find(macro);
    while(i != string::npos)
    {
        str.replace(i, macro.length(), value);
        i = str.find(macro);
    }
}

ShaderInfo getShaderInfo(const filesystem::path& slangInput, const string& suffix, bool fullPath = true)
{
    ShaderInfo info;

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
    info.className  = className.str();
    info.shaderName = shaderName.str();
    info.sourcePath = slangInput;
    info.relativePath = filesystem::path(string(_libName) + "\\" + info.category + "\\" + info.className + suffix + ".h").lexically_normal();
    info.outputPath   = filesystem::path(string(_outputPath) + info.relativePath.string()).lexically_normal();
    filesystem::create_directories(info.outputPath.parent_path());

    replace(info.category, "\\", "/");
    //replace(info.category, "/", "-");
    replace(info.category, " ", "-");
    replace(info.category, "+", "-");
    replace(info.category, "/Mega_Bezel/Presets", "");
    replace(info.category, "Base_CRT_Presets", "Mega_Bezel_Base");

    return info;
}

struct ShaderDef
{
    ShaderDef(const filesystem::path& input) : input {input}, info {getShaderInfo(input, "ShaderDef")}, format{} { }

    filesystem::path    input;
    string              vertexSource;
    string              vertexByteCode;
    string              vertexMetadata;
    string              fragmentSource;
    string              fragmentByteCode;
    string              fragmentMetadata;
    vector<ShaderParam> params;
    ShaderInfo          info;
    string              format;
    map<string, string> presetParams;
    vector<string>      comments;
};

struct TextureDef
{
    TextureDef(const filesystem::path& input) : input {input}, info {getShaderInfo(input, "TextureDef", true)} { }

    filesystem::path    input;
    string              data;
    ShaderInfo          info;
    map<string, string> presetParams;
};
