#include "pch.h"

#pragma once

#pragma comment(lib, "dxguid.lib")

#include "Texture.h"
#include "WIC\WICTextureLoader11.h"

Texture::Texture(TextureDef* textureDef) : m_textureDef(textureDef)
{
    m_name = textureDef->PresetParams["name"];
    std::string value;
    if(Get("linear", value) && value == "true")
        m_linear = true;
    if(Get("mipmap", value) && value == "true")
        m_mipmap = true;
    if(Get("wrap_mode", value))
    {
        if(value == "repeat")
            m_repeat = true;
        if(value == "clamp_to_border")
            m_clamp = true;
        if(value == "mirrored_repeat")
            m_mirror = true;
    }
}

void Texture::Create(ID3D11Device* d3dDevice)
{
    DirectX::CreateWICTextureFromMemory(d3dDevice, m_textureDef->Data, m_textureDef->DataLength, &m_textureResource, &m_textureView, 0);
}

bool Texture::Get(const std::string& presetParam, std::string& value)
{
    auto it = m_textureDef->PresetParams.find(presetParam);
    if(it != m_textureDef->PresetParams.end())
    {
        value = it->second;
        return true;
    }
    return false;
}


Texture::~Texture()
{
    if(m_textureResource)
    {
        m_textureResource->Release();
        m_textureResource = nullptr;
    }
    if(m_textureView)
    {
        m_textureView->Release();
        m_textureView = nullptr;
    }
}
