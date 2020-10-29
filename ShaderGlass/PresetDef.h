#pragma once

class PresetDef
{
public:
    PresetDef() : ShaderDefs {}, TextureDefs {}, Name {}, Category {} { }

    virtual void Build() { }

    std::vector<ShaderDef>  ShaderDefs;
    std::vector<TextureDef> TextureDefs;
    const char*             Name;
    const char*             Category;
};