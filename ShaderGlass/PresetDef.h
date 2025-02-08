#pragma once

class PresetDef
{
public:
    PresetDef() : ShaderDefs {}, TextureDefs {}, Overrides {}, Name {}, Category {} { }

    virtual void Build() { }

    void OverrideParam(const char* name, float value)
    {
        Overrides.emplace_back(name, value);
    }

    std::vector<ShaderDef>     ShaderDefs;
    std::vector<TextureDef>    TextureDefs;
    std::vector<ParamOverride> Overrides;
    const char*                Name;
    const char*                Category;
};