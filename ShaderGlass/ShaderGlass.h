#pragma once

#include "Preset.h"
#include "ShaderPass.h"

class ShaderGlass
{
public:
    ShaderGlass();
    void Initialize(HWND outputWindow, HWND captureWindow, bool clone, ID3D11Device* device);
    void Process(ID3D11Texture2D* texture);
    void SetInputScale(float w, float h);
    void SetOutputScale(float w, float h);
    void SetOutputFlip(bool h, bool v);
    void SetShaderPreset(PresetDef* p);
    void SetFrameSkip(int s);
    void Stop();
    ~ShaderGlass();

private:
    bool TryResizeSwapChain(const RECT& clientRect, bool force);
    void DestroyShaders();
    void DestroyPasses();
    void DestroyTargets();
    void RebuildShaders();

    POINT                     m_lastSize;
    POINT                     m_lastPos;
    POINT                     m_lastCaptureWindowPos;
    ID3D11DeviceContext*      m_context {nullptr};
    ID3D11Device*             m_device {nullptr};
    ID3D11ShaderResourceView* m_originalView {nullptr};
    IDXGISwapChain1*          m_swapChain {nullptr};
    ID3D11RasterizerState*    m_rasterizerState {nullptr};
    ID3D11RenderTargetView*   m_displayRenderTarget {nullptr};
    ID3D11Texture2D*          m_preprocessedTexture {nullptr};
    ID3D11RenderTargetView*   m_preprocessedRenderTarget {nullptr};

    std::vector<ID3D11Texture2D*>                    m_passTextures;
    std::vector<ID3D11RenderTargetView*>             m_passTargets;
    std::map<std::string, ID3D11ShaderResourceView*> m_passResources;
    std::map<std::string, ID3D11ShaderResourceView*> m_presetTextures;
    std::map<std::string, float4>                    m_textureSizes;
    std::vector<ShaderPass*>                         m_shaderPasses;

    HWND        m_outputWindow {0};
    HWND        m_captureWindow {0};
    bool        m_clone {false};
    Shader*     m_preprocessShader {nullptr};
    Preset*     m_shaderPreset {nullptr};
    ShaderPass* m_preprocessPass {nullptr};
    int         m_frameCounter {0};
    bool        m_requiresFeedback {false};
    std::mutex  m_mutex {};

    Preset* volatile m_newShaderPreset {nullptr};
    volatile int   m_frameSkip {0};
    volatile bool  m_running {false};
    volatile float m_inputScaleW {3.0f};
    volatile float m_inputScaleH {3.0f};
    volatile bool  m_inputRescaled {false};
    volatile float m_outputScaleW {1.0f};
    volatile float m_outputScaleH {1.0f};
    volatile bool  m_outputRescaled {false};
    volatile bool  m_flipHorizontal {false};
    volatile bool  m_flipVertical {false};
};
