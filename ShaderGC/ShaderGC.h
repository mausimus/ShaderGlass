/*
ShaderGC: slangp shader compiler for ShaderGlass
Copyright (C) 2021-2025 mausimus (mausimus.net)
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0
*/

#pragma once

#include "PresetDef.h"
#include "SourceDefs.h"
#include "ShaderCache.h"

class file_error : public std::runtime_error
{
public:
    using _Mybase = std::runtime_error;

    explicit file_error(const std::string& _Message) : _Mybase(_Message.c_str()) { }

    explicit file_error(const char* _Message) : _Mybase(_Message) { }
};

class ShaderGC
{
public:
    static PresetDef* CompilePreset(std::filesystem::path source, std::ostream& log, bool& warn, const ShaderCache& cache);
    static TextureDef CompileTexture(std::filesystem::path source, std::ostream& log, bool& warn);

    static std::vector<std::string> LoadSource(const std::filesystem::path& input, bool followIncludes);
    static void                     ProcessSourceShader(SourceShaderDef& def, std::ostream& log, bool& warn);
    static void                     ProcessSourcePreset(SourcePresetDef& def, std::ostream& log, bool& warn);

    static void ParsePreset(const std::filesystem::path& input, std::map<std::string, std::string>& keyValues, std::map<std::string, std::filesystem::path>& valuePaths);

    static std::vector<SourceShaderParam>
    LookupParams(const std::vector<SourceShaderParam>& declaredParams, std::vector<SourceShaderSampler>& textures, const std::string& metadata);

private:
    static ShaderDef  CompileSourceShader(SourceShaderDef& def, std::ostream& log, bool& warn, const ShaderCache& cache);
    static PresetDef* CompileShader(std::filesystem::path source, std::ostream& log, bool& warn, const ShaderCache& cache);

    // Security helpers
    static std::filesystem::path ValidateIncludePath(const std::filesystem::path& basePath, const std::filesystem::path& includePath);
    static std::vector<std::string> LoadSourceWithDepthLimit(const std::filesystem::path& input, bool followIncludes, int depth);
    static void ParsePresetWithDepthLimit(const std::filesystem::path& input,
                                          std::map<std::string, std::string>& keyValues,
                                          std::map<std::string, std::filesystem::path>& valuePaths,
                                          int depth);
    static constexpr int MAX_INCLUDE_DEPTH = 32;
};
