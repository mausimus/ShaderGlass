#include "pch.h"

#include "Shader.h"
#include "Preset.h"

#pragma once

class ShaderPass
{
public:
    ShaderPass(ID3D11Device* device, ID3D11DeviceContext* context, Shader* shader, Preset* preset);
    ~ShaderPass();

    void Initialize();
    void Render(const std::map<std::string, ID3D11ShaderResourceView*>& resources);
    void Render(ID3D11ShaderResourceView* sourceView, const std::map<std::string, ID3D11ShaderResourceView*>& resources);
    void Resize(int sourceWidth, int sourceHeight, int destWidth, int destHeight, const std::map<std::string, float4>& textureSizes);
    void UpdateMVP(float sx, float sy, float tx, float ty);
    bool RequiresFeedback();

    Shader*                   m_shader {nullptr};
    Preset*                   m_preset {nullptr};
    ID3D11ShaderResourceView* m_sourceView {nullptr};
    ID3D11RenderTargetView*   m_targetView {nullptr};
    int                       m_destWidth {0};
    int                       m_destHeight {0};

private:
    float4x4                           m_modelViewProj {};
    ID3D11Device*                      m_device {nullptr};
    ID3D11DeviceContext*               m_context {nullptr};
    ID3D11InputLayout*                 m_inputLayout {nullptr};
    ID3D11Buffer*                      m_vertexBuffer {nullptr};
    ID3D11Buffer*                      m_constantBuffer {nullptr};
    ID3D11Buffer*                      m_pushBuffer {nullptr};
    std::map<int, ID3D11SamplerState*> m_samplers;
    const UINT                         s_vertexStride {6 * sizeof(float)};
    const UINT                         s_vertexOffset {0};
    const UINT                         s_vertexCount {6};
    float                              params_SourceSize[4] {0, 0, 0, 0};
    float                              params_OutputSize[4] {0, 0, 0, 0};
    int                                params_FrameCount {0};
};
