#include "pch.h"

#include "TextureDef.h"

#pragma once

class Texture
{
public:
    TextureDef*               m_textureDef;
    std::string               m_name;
    bool                      m_linear;
    bool                      m_mipmap;
    bool                      m_clamp;
    bool                      m_repeat;
    bool                      m_mirror;
    winrt::com_ptr<ID3D11Resource> m_textureResource;
    winrt::com_ptr<ID3D11ShaderResourceView> m_textureView;

    Texture(TextureDef* textureDef);
    void Create(winrt::com_ptr<ID3D11Device> d3dDevice);
    ~Texture();

private:
    bool Get(const std::string& presetParam, std::string& value);
};