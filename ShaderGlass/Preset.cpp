#include "pch.h"

#include "Preset.h"

Preset::Preset(PresetDef* presetDef) : m_presetDef(presetDef), m_shaders {}
{
    if(presetDef->ShaderDefs.empty())
        presetDef->Build();
}

void Preset::Create(winrt::com_ptr<ID3D11Device> d3dDevice)
{
    for(auto& sd : m_presetDef->ShaderDefs)
    {
        m_shaders.push_back(new Shader(&sd));
    }
    for(auto& td : m_presetDef->TextureDefs)
    {
        auto texture = new Texture(&td);
        m_textures.insert(std::make_pair(texture->m_name, texture));
    }
    for(auto& s : m_shaders)
    {
        s->Create(d3dDevice);
    }
    for(auto& t : m_textures)
    {
        t.second->Create(d3dDevice);
    }
}

Preset::~Preset()
{
    for(auto& s : m_shaders)
    {
        delete s;
    }
    for(auto& t : m_textures)
    {
        delete t.second;
    }
    m_shaders.clear();
    m_textures.clear();
}
