#include "pch.h"

#pragma once

#pragma comment(lib, "dxguid.lib")

#include "Texture.h"
#include "WIC\WICTextureLoader11.h"

Texture::Texture(TextureDef& textureDef) : m_linear(false), m_mipmap(false), m_repeat(false), m_clamp(false), m_mirror(false), m_textureDef(textureDef)
{
    m_name = textureDef.PresetParams["name"];
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

void Texture::Create(winrt::com_ptr<ID3D11Device> d3dDevice)
{
    auto hr = DirectX::CreateWICTextureFromMemoryEx(d3dDevice.get(),
                                                    nullptr,
                                                    m_textureDef.Data,
                                                    m_textureDef.DataLength,
                                                    0,
                                                    D3D11_USAGE_DEFAULT,
                                                    D3D11_BIND_SHADER_RESOURCE,
                                                    0,
                                                    0,
                                                    DirectX::WIC_LOADER_IGNORE_SRGB | DirectX::WIC_LOADER_FORCE_RGBA32,
                                                    m_textureResource.put(),
                                                    m_textureView.put());
}

bool Texture::Get(const std::string& presetParam, std::string& value)
{
    auto it = m_textureDef.PresetParams.find(presetParam);
    if(it != m_textureDef.PresetParams.end())
    {
        value = it->second;
        return true;
    }
    return false;
}

Texture::~Texture()
{
    m_textureView     = nullptr;
    m_textureResource = nullptr;
}
