/*
ShaderGlass: shader effect overlay
Copyright (C) 2021-2025 mausimus (mausimus.net)
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0
*/

#include "pch.h"

#pragma once

#pragma comment(lib, "dxguid.lib")

#include "Texture.h"
#include "WIC\WICTextureLoader11.h"
#include "../ShaderGC/SecurityLimits.h"

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
        if(value == "clamp_to_edge")
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

    // Security: Validate texture dimensions after creation
    if(SUCCEEDED(hr) && m_textureResource)
    {
        winrt::com_ptr<ID3D11Texture2D> tex2D;
        hr = m_textureResource->QueryInterface(IID_PPV_ARGS(tex2D.put()));
        if(SUCCEEDED(hr))
        {
            D3D11_TEXTURE2D_DESC desc;
            tex2D->GetDesc(&desc);

            if(desc.Width > SecurityLimits::MAX_TEXTURE_WIDTH || desc.Height > SecurityLimits::MAX_TEXTURE_HEIGHT)
            {
                // Texture too large - release resources
                m_textureView = nullptr;
                m_textureResource = nullptr;
                throw std::runtime_error("Security: Texture dimensions too large (" +
                                       std::to_string(desc.Width) + "x" + std::to_string(desc.Height) +
                                       ", max " + std::to_string(SecurityLimits::MAX_TEXTURE_WIDTH) + "x" +
                                       std::to_string(SecurityLimits::MAX_TEXTURE_HEIGHT) + ")");
            }
        }
    }
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
