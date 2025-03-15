#include "pch.h"

#include "Shader.h"

static HRESULT hr;

Shader::Shader(ShaderDef& shaderDef) :
    m_shaderDef(shaderDef), m_vertexShader {}, m_pixelShader {}, m_alias {}, m_scaleAbsoluteX {}, m_scaleAbsoluteY {}, m_scaleViewportX {},
    m_scaleViewportY {}
{
    m_pushBuffer = std::make_unique<int[]>(BufferSize(PUSH_BUFFER));
    m_uboBuffer  = std::make_unique<int[]>(BufferSize(UBO_BUFFER));
    for(auto& p : shaderDef.Params)
    {
        SetParam(p.name, &p.defaultValue);
    }

    m_filterLinear = IsTrue("filter_linear");
    m_formatSRGB   = IsTrue("srgb_framebuffer") || (shaderDef.Format != NULL && (strcmp(shaderDef.Format, "R8G8B8A8_SRGB") == 0));
    m_formatFloat  = IsTrue("float_framebuffer") || (shaderDef.Format != NULL && (strcmp(shaderDef.Format, "R32G32B32A32_SFLOAT") == 0 || strcmp(shaderDef.Format, "R16G16B16A16_SFLOAT") == 0));

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
    if(Get("wrap_mode", value))
    {
        if(value == "clamp_to_edge")
            m_clamp = true;
    }
}

void Shader::Create(winrt::com_ptr<ID3D11Device> d3dDevice)
{
    if(m_shaderDef.VertexLength == 0)
        Compile();

    hr = d3dDevice->CreateVertexShader(m_shaderDef.VertexByteCode, m_shaderDef.VertexLength, NULL, m_vertexShader.put());
    assert(SUCCEEDED(hr));

    hr = d3dDevice->CreatePixelShader(m_shaderDef.FragmentByteCode, m_shaderDef.FragmentLength, NULL, m_pixelShader.put());
    assert(SUCCEEDED(hr));
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
}

void Shader::SetParam(std::string name, void* v)
{
    for(auto& p : m_shaderDef.Params)
    {
        if(p.name == name)
        {
            SetParam(&p, v);
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