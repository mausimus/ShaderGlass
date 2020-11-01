#include "pch.h"

#include "Shader.h"

static HRESULT hr;

Shader::Shader(ShaderDef* shaderDef) :
    m_shaderDef(shaderDef), m_vertexShader {}, m_pixelShader {}, m_alias {}, m_scaleAbsoluteX {}, m_scaleAbsoluteY {}, m_scaleViewportX {},
    m_scaleViewportY {}
{
    m_pushBuffer = new int[BufferSize(PUSH_BUFFER)];
    m_uboBuffer  = new int[BufferSize(UBO_BUFFER)];
    for(auto& p : shaderDef->Params)
    {
        SetParam(p.name, &p.defaultValue);
    }

    m_filterLinear = IsTrue("filter_linear");
    m_formatSRGB   = IsTrue("srgb_framebuffer");
    m_formatFloat  = IsTrue("float_framebuffer");

    std::string value;

    if(Get("scale_x", value))
        m_scaleX = stof(value);
    if(Get("scale_y", value))
        m_scaleY = stof(value);
    if(Get("scale", value))
    {
        m_scaleX = stof(value);
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
        m_frameCountMod = static_cast<int>(atof(value.c_str()));
    }
}

void Shader::Create(winrt::com_ptr<ID3D11Device> d3dDevice)
{
    if(m_shaderDef->VertexLength == 0)
        Compile();

    hr = d3dDevice->CreateVertexShader(m_shaderDef->VertexByteCode, m_shaderDef->VertexLength, NULL, m_vertexShader.put());
    assert(SUCCEEDED(hr));

    hr = d3dDevice->CreatePixelShader(m_shaderDef->FragmentByteCode, m_shaderDef->FragmentLength, NULL, m_pixelShader.put());
    assert(SUCCEEDED(hr));
}

void Shader::Compile()
{
    UINT      flags      = 0; // D3DCOMPILE_ENABLE_STRICTNESS;
    ID3DBlob* error_blob = NULL;
    ID3DBlob* vertexBlob = NULL;
    ID3DBlob* pixelBlob  = NULL;

    hr = D3DCompile(m_shaderDef->VertexSource,
                            strlen(m_shaderDef->VertexSource),
                            "Vertex",
                            NULL,
                            D3D_COMPILE_STANDARD_FILE_INCLUDE,
                            "main",
                            "vs_5_0",
                            flags,
                            0,
                            &vertexBlob,
                            &error_blob);
    if(FAILED(hr))
    {
        if(error_blob)
        {
            OutputDebugStringA((char*)error_blob->GetBufferPointer());
            error_blob->Release();
        }
        if(vertexBlob)
        {
            vertexBlob->Release();
        }
        assert(false);
    }

    hr = D3DCompile(m_shaderDef->FragmentSource,
                    strlen(m_shaderDef->FragmentSource),
                    "Fragment",
                    NULL,
                    D3D_COMPILE_STANDARD_FILE_INCLUDE,
                    "main",
                    "ps_5_0",
                    flags,
                    0,
                    &pixelBlob,
                    &error_blob);
    if(FAILED(hr))
    {
        if(error_blob)
        {
            OutputDebugStringA((char*)error_blob->GetBufferPointer());
            error_blob->Release();
        }
        if(pixelBlob)
        {
            pixelBlob->Release();
        }
        assert(false);
    }

    m_shaderDef->VertexByteCode   = (BYTE*)vertexBlob->GetBufferPointer();
    m_shaderDef->VertexLength     = vertexBlob->GetBufferSize();
    m_shaderDef->FragmentByteCode = (BYTE*)pixelBlob->GetBufferPointer();
    m_shaderDef->FragmentLength   = pixelBlob->GetBufferSize();
}

void Shader::FillParams(int buffer, void* data)
{
    if(buffer == PUSH_BUFFER)
        memcpy(data, m_pushBuffer, BufferSize(buffer));
    else
        memcpy(data, m_uboBuffer, BufferSize(buffer));
}

void Shader::SetParam(std::string name, void* v)
{
    for(const auto& p : m_shaderDef->Params)
    {
        if(p.name == name)
        {
            char* buf = (char*)(p.buffer == PUSH_BUFFER ? m_pushBuffer : m_uboBuffer);
            memcpy(buf + p.offset, v, p.size);
            // return; // same param can be in both bufs
        }
    }
}

size_t Shader::BufferSize(int buffer)
{
    return m_shaderDef->ParamsSize(buffer);
}

bool Shader::IsTrue(const std::string& presetParam)
{
    auto it = m_shaderDef->PresetParams.find(presetParam);
    return it != m_shaderDef->PresetParams.end() && (it->second == "true" || it->second == "1");
}

bool Shader::Get(const std::string& presetParam, std::string& value)
{
    auto it = m_shaderDef->PresetParams.find(presetParam);
    if(it != m_shaderDef->PresetParams.end())
    {
        value = it->second;
        return true;
    }
    return false;
}


Shader::~Shader()
{
    delete[] m_pushBuffer;
    delete[] m_uboBuffer;
}