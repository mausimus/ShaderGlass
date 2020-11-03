#include "Shader.h"
#include "Texture.h"
#include "PresetDef.h"

#pragma once

class Preset
{
public:
    Preset(PresetDef& presetDef);
    void Create(winrt::com_ptr<ID3D11Device> d3dDevice);

    PresetDef&                     m_presetDef;
    std::vector<Shader>            m_shaders;
    std::map<std::string, Texture> m_textures;

    ~Preset();
};
