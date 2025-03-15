#pragma once

#include "PresetDef.h"
#include "SourceDefs.h"

class ShaderGC
{
public:
    static PresetDef* CompilePreset(std::filesystem::path source, std::ostream& log, bool& warn);
    static TextureDef CompileTexture(std::filesystem::path source, std::ostream& log, bool& warn);

    static std::vector<std::string> LoadSource(const std::filesystem::path& input, bool followIncludes);
    static void                     ProcessSourceShader(SourceShaderDef& def, std::ostream& log, bool& warn);
    static void                     ProcessSourcePreset(SourcePresetDef& def, std::ostream& log, bool& warn);

    static void ParsePreset(const std::filesystem::path& input, std::map<std::string, std::string>& keyValues, std::map<std::string, std::filesystem::path>& valuePaths);

    static std::vector<SourceShaderParam>
    LookupParams(const std::vector<SourceShaderParam>& declaredParams, std::vector<SourceShaderSampler>& textures, const std::string& metadata);

private:
    static ShaderDef CompileSourceShader(SourceShaderDef& def, std::ostream& log, bool& warn);
    static PresetDef* CompileShader(std::filesystem::path source, std::ostream& log, bool& warn);
};
