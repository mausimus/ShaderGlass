#pragma once

#include "framework.h"
#include "ShaderDef.h"
#include "TextureDef.h"

class PresetDef
{
public:
    PresetDef() : ShaderDefs {}, TextureDefs {}, Overrides {}, Name {}, Category {} { }

    virtual void Build() { }

    void OverrideParam(const char* name, float value)
    {
        Overrides.emplace_back(name, value);
    }

    void MakeDynamic()
    {
        for(auto& s : ShaderDefs)
            s.Dynamic = true;
        for(auto& t : TextureDefs)
            t.Dynamic = true;
    }

    std::vector<ShaderDef>     ShaderDefs;
    std::vector<TextureDef>    TextureDefs;
    std::vector<ParamOverride> Overrides;
    std::string                Name;
    std::string                Category;
};