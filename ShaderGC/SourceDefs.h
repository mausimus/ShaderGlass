#pragma once

#include "framework.h"

static inline void ltrim(std::string& s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch) && ch != '\"'; }));
}

static inline void rtrim(std::string& s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch) && ch != '\"'; }).base(), s.end());
}

static inline std::string trim(std::string s)
{
    ltrim(s);
    rtrim(s);
    return s;
}

struct SourceShaderParam
{
    SourceShaderParam(std::string s, int size, int buffer) : i {-1}, buffer {buffer}, size {size}, offset {}, name {}, desc {}, min {}, max {}, def {}, step {}
    {
        std::istringstream iss(s);

        iss >> name;
        iss >> name;
        iss >> name;
        iss >> std::quoted(desc);
        desc = trim(desc);
        iss >> def;
        iss >> min;
        iss >> max;
        if(!iss.eof())
            iss >> step;
        else
            step = 0;
    }

    SourceShaderParam(std::string s, float o) : SourceShaderParam(s, 4, 0)
    {
        def = o;
    }

    int         buffer; // -1 - push constant, 0 - first UBO, etc.
    int         i;
    int         size;
    int         offset;
    std::string name;
    std::string desc;
    float       min;
    float       max;
    float       def;
    float       step;
};

struct SourceShaderSampler
{
    SourceShaderSampler(std::string name, int binding) : name {name}, binding {binding} { }
    std::string name;
    int         binding;
};

struct SourcePresetTexture
{
    SourcePresetTexture(std::string name) : name {name}, source {}, linear {}, wrap {}, mipmap {} { }
    std::string name;
    std::string source;
    bool        linear;
    bool        wrap;
    bool        mipmap;
};

struct SourceShaderInfo
{
    std::filesystem::path sourcePath;
    std::filesystem::path outputPath;
    std::filesystem::path relativePath;
    std::string           className;
    std::string           shaderName;
    std::string           category;
};

struct SourceShaderDef
{
    SourceShaderDef(const std::filesystem::path& input, SourceShaderInfo info) : input {input}, info {info}, format {} { }

    std::filesystem::path              input;
    std::string                        vertexSource;
    std::string                        vertexByteCode;
    std::string                        vertexMetadata;
    std::string                        fragmentSource;
    std::string                        fragmentByteCode;
    std::string                        fragmentMetadata;
    std::vector<SourceShaderParam>     params;
    SourceShaderInfo                   info;
    std::string                        format;
    std::map<std::string, std::string> presetParams;
    std::vector<std::string>           comments;
};

struct SourceTextureDef
{
    SourceTextureDef(const std::filesystem::path& input, SourceShaderInfo info) : input {input}, info {info} { }

    std::filesystem::path              input;
    std::string                        data;
    SourceShaderInfo                   info;
    std::map<std::string, std::string> presetParams;
};

struct SourcePresetDef
{
    SourcePresetDef(const std::filesystem::path& input, SourceShaderInfo info) : input {input}, info {info} { }

    std::filesystem::path          input;
    std::vector<SourceShaderDef>   shaders;    
    std::vector<SourceTextureDef>  textures;
    std::vector<SourceShaderParam> overrides;
    SourceShaderInfo               info;
};
