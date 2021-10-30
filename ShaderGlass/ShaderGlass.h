#pragma once

#include "Preset.h"
#include "ShaderPass.h"
#include "Shaders\PreprocessShaderDef.h"
#include "Shaders\PassthroughShaderDef.h"
#include "Shaders\PassthroughPresetDef.h"
#include <mutex>

class ShaderGlass
{
public:
    ShaderGlass();
    void                            Initialize(HWND                                outputWindow,
                                               HWND                                captureWindow,
                                               HMONITOR                            captureMonitor,
                                               bool                                clone,
                                               winrt::com_ptr<ID3D11Device>        device,
                                               winrt::com_ptr<ID3D11DeviceContext> context);
    void                            Process(winrt::com_ptr<ID3D11Texture2D> texture);
    void                            SetInputScale(float w, float h);
    void                            SetOutputScale(float w, float h);
    void                            SetOutputFlip(bool h, bool v);
    void                            SetShaderPreset(PresetDef* p, const std::vector<std::tuple<int, std::string, double>>& params);
    void                            SetFrameSkip(int s);
    winrt::com_ptr<ID3D11Texture2D> GrabOutput();
    std::vector<std::tuple<int, ShaderParam*>> Params();
    void                                       UpdateParams();
    void                                       ResetParams();
    void                                       Stop();
    ~ShaderGlass();

private:
    bool TryResizeSwapChain(const RECT& clientRect, bool force);
    void DestroyShaders();
    void DestroyPasses();
    void DestroyTargets();
    void RebuildShaders();

    POINT                                    m_lastSize;
    POINT                                    m_lastPos;
    POINT                                    m_lastCaptureWindowPos;
    winrt::com_ptr<ID3D11DeviceContext>      m_context {nullptr};
    winrt::com_ptr<ID3D11Device>             m_device {nullptr};
    winrt::com_ptr<ID3D11ShaderResourceView> m_originalView {nullptr};
    winrt::com_ptr<IDXGISwapChain1>          m_swapChain {nullptr};
    winrt::com_ptr<ID3D11RasterizerState>    m_rasterizerState {nullptr};
    winrt::com_ptr<ID3D11Texture2D>          m_displayTexture {nullptr};
    winrt::com_ptr<ID3D11RenderTargetView>   m_displayRenderTarget {nullptr};
    winrt::com_ptr<ID3D11Texture2D>          m_preprocessedTexture {nullptr};
    winrt::com_ptr<ID3D11RenderTargetView>   m_preprocessedRenderTarget {nullptr};

    std::vector<winrt::com_ptr<ID3D11Texture2D>>                    m_passTextures;
    std::vector<winrt::com_ptr<ID3D11RenderTargetView>>             m_passTargets;
    std::map<std::string, winrt::com_ptr<ID3D11ShaderResourceView>> m_passResources;
    std::map<std::string, winrt::com_ptr<ID3D11ShaderResourceView>> m_presetTextures;
    std::map<std::string, float4>                                   m_textureSizes;
    std::vector<ShaderPass>                                         m_shaderPasses;

    POINT      m_monitorOffset {0, 0};
    HWND       m_outputWindow {0};
    HWND       m_captureWindow {0};
    bool       m_clone {false};
    int        m_frameCounter {0};
    bool       m_requiresFeedback {false};
    std::mutex m_mutex {};

    PassthroughPresetDef                              m_passthroughDef;
    PreprocessShaderDef                               m_preprocessShaderDef;
    PresetDef                                         m_preprocessPresetDef;
    Preset                                            m_preprocessPreset;
    Shader                                            m_preprocessShader;
    ShaderPass                                        m_preprocessPass;
    std::unique_ptr<Preset>                           m_shaderPreset {nullptr};
    std::unique_ptr<Preset>                           m_newShaderPreset {nullptr};
    std::vector<std::tuple<int, std::string, double>> m_newParams;

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
