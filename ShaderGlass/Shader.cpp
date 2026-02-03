/*
ShaderGlass: shader effect overlay
Copyright (C) 2021-2025 mausimus (mausimus.net)
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0
*/

#include "pch.h"

#include "Shader.h"
#include "../ShaderGC/SafeParsing.h"
#include "Util/ErrorHandling.h"

static HRESULT hr;

const static std::unordered_map<std::string, DXGI_FORMAT> sFormats = {{"R8_UNORM", DXGI_FORMAT_R8_UNORM},
                                                                      {"R8_UINT", DXGI_FORMAT_R8_UINT},
                                                                      {"R8_SINT", DXGI_FORMAT_R8_UINT},
                                                                      {"R8G8_UNORM", DXGI_FORMAT_R8G8_UNORM},
                                                                      {"R8G8_UINT", DXGI_FORMAT_R8G8_UINT},
                                                                      {"R8G8_SINT", DXGI_FORMAT_R8G8_SINT},
                                                                      {"R8G8B8A8_UNORM", DXGI_FORMAT_B8G8R8A8_UNORM},
                                                                      {"R8G8B8A8_UINT", DXGI_FORMAT_R8G8B8A8_UINT},
                                                                      {"R8G8B8A8_SINT", DXGI_FORMAT_R8G8B8A8_SINT},
                                                                      {"R8G8B8A8_SRGB", DXGI_FORMAT_B8G8R8A8_UNORM_SRGB},
                                                                      {"A2B10G10R10_UNORM_PACK32", DXGI_FORMAT_R10G10B10A2_UNORM},
                                                                      {"A2B10G10R10_UINT_PACK32", DXGI_FORMAT_R10G10B10A2_UINT},
                                                                      {"R16_UINT", DXGI_FORMAT_R16_UINT},
                                                                      {"R16_SINT", DXGI_FORMAT_R16_SINT},
                                                                      {"R16_SFLOAT", DXGI_FORMAT_R16_FLOAT},
                                                                      {"R16G16_UINT", DXGI_FORMAT_R16G16_UINT},
                                                                      {"R16G16_SINT", DXGI_FORMAT_R16G16_SINT},
                                                                      {"R16G16_SFLOAT", DXGI_FORMAT_R16G16_FLOAT},
                                                                      {"R16G16B16A16_UINT", DXGI_FORMAT_R16G16B16A16_UINT},
                                                                      {"R16G16B16A16_SINT", DXGI_FORMAT_R16G16B16A16_SINT},
                                                                      {"R16G16B16A16_SFLOAT", DXGI_FORMAT_R16G16B16A16_FLOAT},
                                                                      {"R32_UINT", DXGI_FORMAT_R32_UINT},
                                                                      {"R32_SINT", DXGI_FORMAT_R32_SINT},
                                                                      {"R32_SFLOAT", DXGI_FORMAT_R32_FLOAT},
                                                                      {"R32G32_UINT", DXGI_FORMAT_R32G32_UINT},
                                                                      {"R32G32_SINT", DXGI_FORMAT_R32G32_SINT},
                                                                      {"R32G32_SFLOAT", DXGI_FORMAT_R32G32_FLOAT},
                                                                      {"R32G32B32A32_UINT", DXGI_FORMAT_R32G32B32A32_UINT},
                                                                      {"R32G32B32A32_SINT", DXGI_FORMAT_R32G32B32A32_SINT},
                                                                      {"R32G32B32A32_SFLOAT", DXGI_FORMAT_R32G32B32A32_FLOAT}};

Shader::Shader(ShaderDef& shaderDef) :
    m_shaderDef(shaderDef), m_vertexShader {}, m_pixelShader {}, m_alias {}, m_scaleAbsoluteX {}, m_scaleAbsoluteY {}, m_scaleViewportX {}, m_scaleViewportY {}
{
    m_pushBuffer = std::make_unique<int[]>(BufferSize(PUSH_BUFFER));
    m_uboBuffer  = std::make_unique<int[]>(BufferSize(UBO_BUFFER));
    for(auto& p : shaderDef.Params)
    {
        SetParam(p.name, &p.defaultValue);
    }

    m_filterLinear = IsTrue("filter_linear");
    if(IsTrue("srgb_framebuffer"))
        m_format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
    if(IsTrue("float_framebuffer"))
        m_format = DXGI_FORMAT_R16G16B16A16_FLOAT;
    if(shaderDef.Format != NULL && strlen(shaderDef.Format) > 0)
    {
        auto format = sFormats.find(shaderDef.Format);
        if(format != sFormats.end())
            m_format = format->second;
#ifdef _DEBUG
        else
            throw std::runtime_error("Unknown format " + std::string(shaderDef.Format));
#endif
    }

    std::string value;

    if(Get("scale_x", value))
        m_scaleX = SafeParseFloat<float>(value, 1.0f);
    if(Get("scale_y", value))
        m_scaleY = SafeParseFloat<float>(value, 1.0f);
    if(Get("scale", value))
    {
        m_scaleX = SafeParseFloat<float>(value, 1.0f);
        m_scaleY = m_scaleX;
    }

    if(Get("scale_type_x", value))
    {
        if(value == "viewport")
            m_scaleViewportX = true;
        else if(value == "absolute")
            m_scaleAbsoluteX = true;
    }
    if(Get("scale_type_y", value))
    {
        if(value == "viewport")
            m_scaleViewportY = true;
        else if(value == "absolute")
            m_scaleAbsoluteY = true;
    }
    if(Get("scale_type", value))
    {
        if(value == "viewport")
        {
            m_scaleViewportX = true;
            m_scaleViewportY = true;
        }
        else if(value == "absolute")
        {
            m_scaleAbsoluteX = true;
            m_scaleAbsoluteY = true;
        }
    }
    if(Get("alias", value))
    {
        m_alias = value;
    }
    if(Get("framecount_mod", value))
    {
        m_frameCountMod = SafeParseInt<int>(value, 0);
    }
    if(Get("wrap_mode", value))
    {
        if(value == "clamp_to_edge")
            m_clamp = true;
        else if(value == "repeat")
            m_repeat = true;
        else if(value == "mirrored_repeat")
            m_mirror = true;
    }
}

void Shader::Create(winrt::com_ptr<ID3D11Device> d3dDevice)
{
    if(m_shaderDef.VertexLength == 0)
        Compile();

    hr = d3dDevice->CreateVertexShader(m_shaderDef.VertexByteCode, m_shaderDef.VertexLength, NULL, m_vertexShader.put());
    THROW_IF_FAILED(hr);

    hr = d3dDevice->CreatePixelShader(m_shaderDef.FragmentByteCode, m_shaderDef.FragmentLength, NULL, m_pixelShader.put());
    THROW_IF_FAILED(hr);
}

void Shader::Compile()
{
    UINT                     flags = 0; // D3DCOMPILE_ENABLE_STRICTNESS;
    winrt::com_ptr<ID3DBlob> errorBlob;

    hr = D3DCompile(m_shaderDef.VertexSource,
                    strlen(m_shaderDef.VertexSource),
                    "Vertex",
                    NULL,
                    D3D_COMPILE_STANDARD_FILE_INCLUDE,
                    "main",
                    "vs_5_0",
                    flags,
                    0,
                    m_vertexBlob.put(),
                    errorBlob.put());
    if(FAILED(hr))
    {
        if(errorBlob)
        {
            OutputDebugStringA((char*)errorBlob->GetBufferPointer());
        }
        assert(false);
    }

    hr = D3DCompile(m_shaderDef.FragmentSource,
                    strlen(m_shaderDef.FragmentSource),
                    "Fragment",
                    NULL,
                    D3D_COMPILE_STANDARD_FILE_INCLUDE,
                    "main",
                    "ps_5_0",
                    flags,
                    0,
                    m_pixelBlob.put(),
                    errorBlob.put());
    if(FAILED(hr))
    {
        if(errorBlob)
        {
            OutputDebugStringA((char*)errorBlob->GetBufferPointer());
        }
        assert(false);
    }

    m_shaderDef.VertexByteCode   = (BYTE*)m_vertexBlob->GetBufferPointer();
    m_shaderDef.VertexLength     = m_vertexBlob->GetBufferSize();
    m_shaderDef.FragmentByteCode = (BYTE*)m_pixelBlob->GetBufferPointer();
    m_shaderDef.FragmentLength   = m_pixelBlob->GetBufferSize();
}

void Shader::FillParams(int buffer, void* data)
{
    if(buffer == PUSH_BUFFER)
        memcpy(data, m_pushBuffer.get(), BufferSize(buffer));
    else
        memcpy(data, m_uboBuffer.get(), BufferSize(buffer));
}

std::vector<ShaderParam*> Shader::Params()
{
    std::vector<ShaderParam*> params;
    for(auto& p : m_shaderDef.Params)
        params.push_back(&p);
    return params;
}

void Shader::SetParam(ShaderParam* p, void* v)
{
    char* buf = (char*)(p->buffer == PUSH_BUFFER ? m_pushBuffer.get() : m_uboBuffer.get());

    // if it's float remember value (user parameter)
    if(p->size == 4)
        p->currentValue = *((float*)v);

    memcpy(buf + p->offset, v, p->size);

    // Performance: Mark buffer as dirty to trigger upload
    MarkDirty(p->buffer);
}

void Shader::SetParam(std::string name, void* v)
{
    for(auto& p : m_shaderDef.Params)
    {
        if(p.name == name)
        {
            SetParam(&p, v);
            // Note: MarkDirty is called inside SetParam
            // return; // same param can be in both bufs
        }
    }
}

size_t Shader::BufferSize(int buffer)
{
    return m_shaderDef.ParamsSize(buffer);
}

bool Shader::IsTrue(const std::string& presetParam)
{
    auto it = m_shaderDef.PresetParams.find(presetParam);
    return it != m_shaderDef.PresetParams.end() && (it->second == "true" || it->second == "1");
}

bool Shader::Get(const std::string& presetParam, std::string& value)
{
    auto it = m_shaderDef.PresetParams.find(presetParam);
    if(it != m_shaderDef.PresetParams.end())
    {
        value = it->second;
        return true;
    }
    return false;
}

Shader::Shader(Shader&& shader) : m_shaderDef(shader.m_shaderDef)
{
    throw std::runtime_error("This shouldn't happen");
}

Shader::~Shader()
{
    m_pixelShader  = nullptr;
    m_vertexShader = nullptr;
    m_vertexBlob   = nullptr;
    m_pixelBlob    = nullptr;
}