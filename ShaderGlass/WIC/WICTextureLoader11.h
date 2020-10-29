//--------------------------------------------------------------------------------------
// File: WICTextureLoader11.h
//
// Function for loading a WIC image and creating a Direct3D runtime texture for it
// (auto-generating mipmaps if possible)
//
// Note: Assumes application has already called CoInitializeEx
//
// Warning: CreateWICTexture* functions are not thread-safe if given a d3dContext instance for
//          auto-gen mipmap support.
//
// Note these functions are useful for images created as simple 2D textures. For
// more complex resources, DDSTextureLoader is an excellent light-weight runtime loader.
// For a full-featured DDS file reader, writer, and texture processing pipeline see
// the 'Texconv' sample and the 'DirectXTex' library.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkId=248926
// http://go.microsoft.com/fwlink/?LinkId=248929
//--------------------------------------------------------------------------------------

#pragma once

#include <d3d11_1.h>

#include <cstdint>

namespace DirectX
{
#ifndef WIC_LOADER_FLAGS_DEFINED
#    define WIC_LOADER_FLAGS_DEFINED
enum WIC_LOADER_FLAGS : uint32_t
{
    WIC_LOADER_DEFAULT      = 0,
    WIC_LOADER_FORCE_SRGB   = 0x1,
    WIC_LOADER_IGNORE_SRGB  = 0x2,
    WIC_LOADER_SRGB_DEFAULT = 0x4,
    WIC_LOADER_FIT_POW2     = 0x20,
    WIC_LOADER_MAKE_SQUARE  = 0x40,
    WIC_LOADER_FORCE_RGBA32 = 0x80,
};
#endif

// Standard version
HRESULT CreateWICTextureFromMemory(_In_ ID3D11Device*                           d3dDevice,
                                   _In_reads_bytes_(wicDataSize) const uint8_t* wicData,
                                   _In_ size_t                                  wicDataSize,
                                   _Outptr_opt_ ID3D11Resource** texture,
                                   _Outptr_opt_ ID3D11ShaderResourceView** textureView,
                                   _In_ size_t                             maxsize = 0) noexcept;

HRESULT CreateWICTextureFromFile(_In_ ID3D11Device* d3dDevice,
                                 _In_z_ const wchar_t* szFileName,
                                 _Outptr_opt_ ID3D11Resource** texture,
                                 _Outptr_opt_ ID3D11ShaderResourceView** textureView,
                                 _In_ size_t                             maxsize = 0) noexcept;

// Standard version with optional auto-gen mipmap support
HRESULT CreateWICTextureFromMemory(_In_ ID3D11Device* d3dDevice,
                                   _In_opt_ ID3D11DeviceContext*                d3dContext,
                                   _In_reads_bytes_(wicDataSize) const uint8_t* wicData,
                                   _In_ size_t                                  wicDataSize,
                                   _Outptr_opt_ ID3D11Resource** texture,
                                   _Outptr_opt_ ID3D11ShaderResourceView** textureView,
                                   _In_ size_t                             maxsize = 0) noexcept;

HRESULT CreateWICTextureFromFile(_In_ ID3D11Device* d3dDevice,
                                 _In_opt_ ID3D11DeviceContext* d3dContext,
                                 _In_z_ const wchar_t* szFileName,
                                 _Outptr_opt_ ID3D11Resource** texture,
                                 _Outptr_opt_ ID3D11ShaderResourceView** textureView,
                                 _In_ size_t                             maxsize = 0) noexcept;

// Extended version
HRESULT CreateWICTextureFromMemoryEx(_In_ ID3D11Device*                           d3dDevice,
                                     _In_reads_bytes_(wicDataSize) const uint8_t* wicData,
                                     _In_ size_t                                  wicDataSize,
                                     _In_ size_t                                  maxsize,
                                     _In_ D3D11_USAGE                             usage,
                                     _In_ unsigned int                            bindFlags,
                                     _In_ unsigned int                            cpuAccessFlags,
                                     _In_ unsigned int                            miscFlags,
                                     _In_ unsigned int                            loadFlags,
                                     _Outptr_opt_ ID3D11Resource** texture,
                                     _Outptr_opt_ ID3D11ShaderResourceView** textureView) noexcept;

HRESULT CreateWICTextureFromFileEx(_In_ ID3D11Device* d3dDevice,
                                   _In_z_ const wchar_t* szFileName,
                                   _In_ size_t           maxsize,
                                   _In_ D3D11_USAGE      usage,
                                   _In_ unsigned int     bindFlags,
                                   _In_ unsigned int     cpuAccessFlags,
                                   _In_ unsigned int     miscFlags,
                                   _In_ unsigned int     loadFlags,
                                   _Outptr_opt_ ID3D11Resource** texture,
                                   _Outptr_opt_ ID3D11ShaderResourceView** textureView) noexcept;

// Extended version with optional auto-gen mipmap support
HRESULT CreateWICTextureFromMemoryEx(_In_ ID3D11Device* d3dDevice,
                                     _In_opt_ ID3D11DeviceContext*                d3dContext,
                                     _In_reads_bytes_(wicDataSize) const uint8_t* wicData,
                                     _In_ size_t                                  wicDataSize,
                                     _In_ size_t                                  maxsize,
                                     _In_ D3D11_USAGE                             usage,
                                     _In_ unsigned int                            bindFlags,
                                     _In_ unsigned int                            cpuAccessFlags,
                                     _In_ unsigned int                            miscFlags,
                                     _In_ unsigned int                            loadFlags,
                                     _Outptr_opt_ ID3D11Resource** texture,
                                     _Outptr_opt_ ID3D11ShaderResourceView** textureView) noexcept;

HRESULT CreateWICTextureFromFileEx(_In_ ID3D11Device* d3dDevice,
                                   _In_opt_ ID3D11DeviceContext* d3dContext,
                                   _In_z_ const wchar_t* szFileName,
                                   _In_ size_t           maxsize,
                                   _In_ D3D11_USAGE      usage,
                                   _In_ unsigned int     bindFlags,
                                   _In_ unsigned int     cpuAccessFlags,
                                   _In_ unsigned int     miscFlags,
                                   _In_ unsigned int     loadFlags,
                                   _Outptr_opt_ ID3D11Resource** texture,
                                   _Outptr_opt_ ID3D11ShaderResourceView** textureView) noexcept;
} // namespace DirectX