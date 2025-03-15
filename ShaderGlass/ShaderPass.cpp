#include "pch.h"

#include "ShaderPass.h"
#include "Helpers.h"

static HRESULT hr;

ShaderPass::ShaderPass(Shader& shader, Preset& preset, bool preprocess) : m_shader {shader}, m_preset {preset}, m_preprocess {preprocess} { }

ShaderPass::ShaderPass(Shader& shader, Preset& preset, winrt::com_ptr<ID3D11Device> device, winrt::com_ptr<ID3D11DeviceContext> context) :
    ShaderPass(shader, preset, false)
{
    Initialize(device, context);
}

// clang-format off
static float sVertexBuffer[] = 
{
    // Preprocess VB
    -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    -1.0f, 1.0f,  0.0f, 1.0f, 0.0f, 0.0f,
    1.0f,  -1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    1.0f,  1.0f,  0.0f, 1.0f, 1.0f, 0.0f,
    // Shader VB
    0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f
};
// clang-format on

void ShaderPass::Initialize(winrt::com_ptr<ID3D11Device> device, winrt::com_ptr<ID3D11DeviceContext> context)
{
    m_device  = device;
    m_context = context;

    D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}};

    hr = m_device->CreateInputLayout(inputElementDesc,
                                     ARRAYSIZE(inputElementDesc),
                                     m_shader.m_shaderDef.VertexByteCode,
                                     m_shader.m_shaderDef.VertexLength,
                                     m_inputLayout.put());
    assert(SUCCEEDED(hr));
    {
        D3D11_BUFFER_DESC vertex_buff_descr = {};
        vertex_buff_descr.ByteWidth         = sizeof(sVertexBuffer);
        vertex_buff_descr.Usage             = D3D11_USAGE_DEFAULT;
        vertex_buff_descr.BindFlags         = D3D11_BIND_VERTEX_BUFFER;
        D3D11_SUBRESOURCE_DATA sr_data      = {0};
        sr_data.pSysMem                     = sVertexBuffer;
        hr                                  = m_device->CreateBuffer(&vertex_buff_descr, &sr_data, m_vertexBuffer.put());
        assert(SUCCEEDED(hr));
    }

    for(const auto& texture : m_shader.m_shaderDef.Samplers)
    {
        winrt::com_ptr<ID3D11SamplerState> samplerState;
        D3D11_SAMPLER_DESC                 samplerDesc = {};

        samplerDesc.Filter         = D3D11_FILTER_MIN_MAG_MIP_POINT;
        samplerDesc.AddressU       = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerDesc.AddressV       = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerDesc.AddressW       = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerDesc.BorderColor[0] = 0.0f;
        samplerDesc.BorderColor[1] = 0.0f;
        samplerDesc.BorderColor[2] = 0.0f;
        samplerDesc.BorderColor[3] = 0.0f;
        samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

        // is it a static texture?
        //if(m_preset)
        {
            auto ti = m_preset.m_textures.find(texture.name);
            if(ti != m_preset.m_textures.end())
            {
                if(ti->second.m_linear)
                    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
                if(ti->second.m_repeat)
                {
                    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
                    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
                    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
                }
                if(ti->second.m_clamp)
                {
                    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
                    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
                    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
                }
                if(ti->second.m_mirror)
                {
                    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
                    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
                    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
                }
            }
            else
            {
                if(m_shader.m_filterLinear)
                    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
                if(m_shader.m_clamp)
                {
                    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
                    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
                    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
                }
            }
        }

        m_device->CreateSamplerState(&samplerDesc, samplerState.put());
        m_samplers.insert(std::make_pair(texture.binding, samplerState));
    }

    if(m_shader.BufferSize(0) > 0)
    {
        D3D11_BUFFER_DESC constantBufferDesc = {};
        constantBufferDesc.ByteWidth         = (m_shader.BufferSize(0) + 0xf) & 0xfffffff0;
        constantBufferDesc.Usage             = D3D11_USAGE_DYNAMIC;
        constantBufferDesc.BindFlags         = D3D11_BIND_CONSTANT_BUFFER;
        constantBufferDesc.CPUAccessFlags    = D3D11_CPU_ACCESS_WRITE;

        hr = m_device->CreateBuffer(&constantBufferDesc, nullptr, m_constantBuffer.put());
        assert(SUCCEEDED(hr));
    }
    else
    {
        m_constantBuffer = nullptr;
    }

    if(m_shader.BufferSize(-1) > 0)
    {
        D3D11_BUFFER_DESC pushBufferDesc = {};
        pushBufferDesc.ByteWidth         = (m_shader.BufferSize(-1) + 0xf) & 0xfffffff0;
        pushBufferDesc.Usage             = D3D11_USAGE_DYNAMIC;
        pushBufferDesc.BindFlags         = D3D11_BIND_CONSTANT_BUFFER;
        pushBufferDesc.CPUAccessFlags    = D3D11_CPU_ACCESS_WRITE;

        hr = m_device->CreateBuffer(&pushBufferDesc, nullptr, m_pushBuffer.put());
        assert(SUCCEEDED(hr));
    }
    else
    {
        m_pushBuffer = nullptr;
    }

    // create MVP
    memset(&m_modelViewProj, 0, 16 * sizeof(float));
    m_modelViewProj.m[0][0] = 2.0f;
    m_modelViewProj.m[1][1] = 2.0f;
    m_modelViewProj.m[3][0] = -1.0f;
    m_modelViewProj.m[3][1] = -1.0f;
    m_modelViewProj.m[3][3] = 1.0f;
}

void ShaderPass::UpdateMVP(float sx, float sy, float tx, float ty)
{
    // has effect only on preprocess pass
    m_modelViewProj.m[0][0] = sx;
    m_modelViewProj.m[1][1] = sy;
    m_modelViewProj.m[3][0] = tx;
    m_modelViewProj.m[3][1] = ty;
}

ShaderPass::~ShaderPass()
{
    m_inputLayout    = nullptr;
    m_vertexBuffer   = nullptr;
    m_constantBuffer = nullptr;
    m_pushBuffer     = nullptr;
    m_samplers.clear();
}

void ShaderPass::Resize(int sourceWidth, int sourceHeight, int destWidth, int destHeight, const std::map<std::string, float4>& textureSizes, const std::vector<std::array<UINT, 4>>& passSizes)
{
    m_destWidth  = destWidth;
    m_destHeight = destHeight;

    params_SourceSize[0] = static_cast<float>(sourceWidth);
    params_SourceSize[1] = static_cast<float>(sourceHeight);
    params_SourceSize[2] = 1.0f / sourceWidth;
    params_SourceSize[3] = 1.0f / sourceHeight;
    params_OutputSize[0] = static_cast<float>(destWidth);
    params_OutputSize[1] = static_cast<float>(destHeight);
    params_OutputSize[2] = 1.0f / destWidth;
    params_OutputSize[3] = 1.0f / destHeight;
    m_shader.SetParam("SourceSize", params_SourceSize);
    m_shader.SetParam("OutputSize", params_OutputSize);

    for(const auto& tx : textureSizes)
    {
        auto sizeParam = tx.first + "Size";
        m_shader.SetParam(sizeParam, (void*)&tx.second);
    }
    for(auto p = 0; p < passSizes.size(); p++)
    {
        const auto& passSize = passSizes.at(p);
        if(passSize[2] != 0 && passSize[3] != 0)
        {
            auto sizeParam = "PassOutputSize" + std::to_string(p);
            float passSizeF[4] = { passSize[2], passSize[3], 1.0f / passSize[2], 1.0f / passSize[3] };
            m_shader.SetParam(sizeParam, passSizeF);
        }
    }
}

void ShaderPass::Render(std::map<std::string, winrt::com_ptr<ID3D11ShaderResourceView>>& resources, int frameCount, int boxX, int boxY)
{
    Render(m_sourceView, resources, frameCount, boxX, boxY);
}

void ShaderPass::Render(ID3D11ShaderResourceView* sourceView, std::map<std::string, winrt::com_ptr<ID3D11ShaderResourceView>>& resources, int frameCount, int boxX, int boxY)
{
    params_FrameCount += frameCount;
    if(m_shader.m_frameCountMod > 0)
    {
        while(params_FrameCount >= m_shader.m_frameCountMod)
            params_FrameCount -= m_shader.m_frameCountMod;
    }

    m_shader.SetParam("FrameCount", &params_FrameCount);
    m_shader.SetParam("MVP", &m_modelViewProj);

    if(m_constantBuffer != nullptr)
    {
        D3D11_MAPPED_SUBRESOURCE mappedSubresource;
        m_context->Map(m_constantBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
        m_shader.FillParams(0, (char*)mappedSubresource.pData);
        m_context->Unmap(m_constantBuffer.get(), 0);
    }

    if(m_pushBuffer != nullptr)
    {
        D3D11_MAPPED_SUBRESOURCE mappedSubresource;
        m_context->Map(m_pushBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
        m_shader.FillParams(-1, (char*)mappedSubresource.pData);
        m_context->Unmap(m_pushBuffer.get(), 0);
    }

    D3D11_VIEWPORT viewport = {static_cast<float>(boxX), static_cast<float>(boxY), static_cast<float>(m_destWidth), static_cast<float>(m_destHeight), 0.0f, 1.0f};
    m_context->RSSetViewports(1, &viewport);

    ID3D11RenderTargetView* targets[1] = {m_targetView};
    m_context->OMSetRenderTargets(1, targets, NULL);

    m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    m_context->IASetInputLayout(m_inputLayout.get());
    ID3D11Buffer* vertexBuffer[1] = {m_vertexBuffer.get()};
    m_context->IASetVertexBuffers(0, 1, vertexBuffer, &s_vertexStride, &s_vertexOffset);

    m_context->VSSetShader(m_shader.m_vertexShader.get(), NULL, 0);
    m_context->PSSetShader(m_shader.m_pixelShader.get(), NULL, 0);

    std::vector<int> bindings;
    for(const auto& texture : m_shader.m_shaderDef.Samplers)
    {
        auto& sampler = m_samplers.at(texture.binding);
        if(texture.name == "Source")
        {
            ID3D11ShaderResourceView* localResources[1] = {sourceView};
            m_context->PSSetShaderResources(texture.binding, 1, localResources);
            bindings.push_back(texture.binding);
        }
        else
        {
            auto it = resources.find(texture.name);
            if(it == resources.end() && texture.name.starts_with("OriginalHistory"))
            {
                it = resources.find("Original"); // should only map 0 to Original
            }
            if(it != resources.end())
            {
                ID3D11ShaderResourceView* localResources[1] = {it->second.get()};
                m_context->PSSetShaderResources(texture.binding, 1, localResources);
                bindings.push_back(texture.binding);
            }
            else
            {
#ifdef _DEBUG
                OutputDebugStringW(convertCharArrayToLPCWSTR(texture.name.c_str()));
                OutputDebugStringW(L"\n");
#endif
            }
        }
        ID3D11SamplerState* samplers[1] = {sampler.get()};
        m_context->PSSetSamplers(texture.binding, 1, samplers);
    }

    if(m_constantBuffer != nullptr)
    {
        ID3D11Buffer* buffer[1] = {m_constantBuffer.get()};
        m_context->VSSetConstantBuffers(0, 1, buffer);
        m_context->PSSetConstantBuffers(0, 1, buffer);
    }
    if(m_pushBuffer != nullptr)
    {
        ID3D11Buffer* buffer[1] = {m_pushBuffer.get()};
        m_context->VSSetConstantBuffers(1, 1, buffer);
        m_context->PSSetConstantBuffers(1, 1, buffer);
    }

    if(this->m_shader.m_shaderDef.Name == "preprocess")
    {
        m_context->Draw(s_vertexCount, 0);
    }
    else
    {
        m_context->Draw(s_vertexCount, 4);
    }

    // unbind to allow rebinding as input/output
    for(auto& b : bindings)
    {
        ID3D11ShaderResourceView* null[] = {nullptr};
        m_context->PSSetShaderResources(b, 1, null);
    }
    ID3D11RenderTargetView* null[] = {nullptr};
    m_context->OMSetRenderTargets(1, null, NULL);
}

bool ShaderPass::RequiresFeedback() const
{
    for(const auto& texture : m_shader.m_shaderDef.Samplers)
    {
        if(texture.name.ends_with("Feedback") || texture.name.starts_with("PassFeedback"))
        {
            return true;
        }
    }
    return false;
}

int ShaderPass::RequiresHistory() const
{
    int maxHistory = 0;
    for(const auto& texture : m_shader.m_shaderDef.Samplers)
    {
        if(texture.name.starts_with("OriginalHistory"))
        {
            try
            {
                auto historyString = texture.name.substr(15);
                auto historyNum    = std::stoi(historyString);
                if(historyNum > 0 && historyNum < 100)
                {
                    maxHistory = max(maxHistory, historyNum);
                }
            }
            catch(...)
            { }
        }
    }
    return maxHistory;
}
