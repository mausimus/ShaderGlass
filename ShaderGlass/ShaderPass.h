#include "pch.h"

#include "Shader.h"
#include "Preset.h"

#pragma once

class ShaderPass
{
public:
    ShaderPass(Shader& shader, Preset& preset, bool preprocess);
    ShaderPass(Shader& shader, Preset& preset, winrt::com_ptr<ID3D11Device> device, winrt::com_ptr<ID3D11DeviceContext> context);
    ~ShaderPass();

    void Initialize(winrt::com_ptr<ID3D11Device> device, winrt::com_ptr<ID3D11DeviceContext> context);
    void Render(std::map<std::string, winrt::com_ptr<ID3D11ShaderResourceView>>& resources, int frameCount);
    void Render(ID3D11ShaderResourceView* sourceView, std::map<std::string, winrt::com_ptr<ID3D11ShaderResourceView>>& resources, int frameCount);
    void Resize(int sourceWidth, int sourceHeight, int destWidth, int destHeight, const std::map<std::string, float4>& textureSizes);
    void UpdateMVP(float sx, float sy, float tx, float ty);
    bool RequiresFeedback() const;

    Shader&                   m_shader;
    Preset&                   m_preset;
    ID3D11ShaderResourceView* m_sourceView {nullptr};
    ID3D11RenderTargetView*   m_targetView {nullptr};
    int                       m_destWidth {0};
    int                       m_destHeight {0};

private:
    float4x4                                          m_modelViewProj {};
    winrt::com_ptr<ID3D11Device>                      m_device {nullptr};
    winrt::com_ptr<ID3D11DeviceContext>               m_context {nullptr};
    winrt::com_ptr<ID3D11InputLayout>                 m_inputLayout {nullptr};
    winrt::com_ptr<ID3D11Buffer>                      m_vertexBuffer {nullptr};
    winrt::com_ptr<ID3D11Buffer>                      m_constantBuffer {nullptr};
    winrt::com_ptr<ID3D11Buffer>                      m_pushBuffer {nullptr};
    std::map<int, winrt::com_ptr<ID3D11SamplerState>> m_samplers;
    bool                                              m_preprocess {false};
    const UINT                                        s_vertexStride {6 * sizeof(float)};
    const UINT                                        s_vertexOffset {0};
    const UINT                                        s_vertexCount {4};
    float                                             params_SourceSize[4] {0, 0, 0, 0};
    float                                             params_OutputSize[4] {0, 0, 0, 0};
    int                                               params_FrameCount {0};
};
