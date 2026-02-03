/*
ShaderGlass: shader effect overlay
Copyright (C) 2021-2025 mausimus (mausimus.net)
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0
*/

#pragma once

#include "Preset.h"
#include "ShaderPass.h"
#include "Shaders\PreprocessShaderDef.h"
#include "Shaders\PassthroughShaderDef.h"
#include "Shaders\PassthroughPresetDef.h"
#include <atomic>

class CursorEmulator;

class ShaderGlass
{
public:
    ShaderGlass(CursorEmulator& cursorEmulator);
    void  Initialize(HWND                                outputWindow,
                     HWND                                captureWindow,
                     HMONITOR                            captureMonitor,
                     bool                                clone,
                     bool                                image,
                     bool                                flipMode,
                     bool                                allowTearing,
                     bool                                useHDR,
                     winrt::com_ptr<ID3D11Device>        device,
                     winrt::com_ptr<ID3D11DeviceContext> context,
                     std::mutex&                         contextMutex);
    void  Process(winrt::com_ptr<ID3D11Texture2D> texture, ULONGLONG frameTicks, int inputFrameNo);
    void  SetInputScale(float w, float h);
    void  SetOutputScale(float w, float h);
    void  SetOutputFlip(bool h, bool v);
    void  SetShaderPreset(PresetDef* p, const std::vector<std::tuple<int, std::string, double>>& params);
    void  SetFrameSkip(int s);
    void  SetLockedArea(RECT area);
    void  SetCroppedArea(RECT area);
    void  SetFreeScale(bool freeScale);
    void  SetVertical(bool vertical);
    float FPS()
    {
        return m_fps;
    }
    winrt::com_ptr<ID3D11Texture2D>            GrabOutput();
    std::vector<std::tuple<int, ShaderParam*>> Params();
    void                                       UpdateParams();
    void                                       ResetParams();
    float                                      GetDefaultValue(ShaderParam* p);
    void                                       Stop();
    ~ShaderGlass();

private:
    bool TryResizeSwapChain(const RECT& clientRect, bool force);
    void SetSwapchainColorSpace();
    void DestroyShaders();
    void DestroyPasses();
    void DestroyTargets();
    void RebuildShaders();
    void PresentFrame();

    POINT                                    m_lastSize;
    POINT                                    m_lastPos;
    POINT                                    m_lastCaptureWindowPos;
    POINT                                    m_lastCaptureWindowSize;
    winrt::com_ptr<ID3D11DeviceContext>      m_context {nullptr};
    std::mutex*                              m_contextMutex {nullptr}; // Thread safety for context (not owned)
    winrt::com_ptr<ID3D11Device>             m_device {nullptr};
    winrt::com_ptr<ID3D11ShaderResourceView> m_originalView {nullptr};

    // Performance: Cache input texture view to avoid recreating every frame
    winrt::com_ptr<ID3D11Texture2D>          m_cachedInputTexture {nullptr};
    winrt::com_ptr<ID3D11ShaderResourceView> m_cachedInputView {nullptr};
    winrt::com_ptr<IDXGISwapChain1>          m_swapChain {nullptr};
    winrt::com_ptr<IDXGISwapChain3>          m_swapChain3 {nullptr};
    winrt::com_ptr<ID3D11RasterizerState>    m_rasterizerState {nullptr};
    winrt::com_ptr<ID3D11Texture2D>          m_displayTexture {nullptr};
    winrt::com_ptr<ID3D11RenderTargetView>   m_displayRenderTarget {nullptr};
    winrt::com_ptr<ID3D11Texture2D>          m_preprocessedTexture {nullptr};
    winrt::com_ptr<ID3D11RenderTargetView>   m_preprocessedRenderTarget {nullptr};

    std::vector<winrt::com_ptr<ID3D11Texture2D>>                    m_passTextures;
    std::vector<winrt::com_ptr<ID3D11RenderTargetView>>             m_passTargets;
    std::map<std::string, winrt::com_ptr<ID3D11ShaderResourceView>> m_passResources;

    // Performance: Pre-generated string keys to avoid allocations in hot path
    std::vector<std::string> m_passOutputKeys;   // "PassOutput0", "PassOutput1", etc.
    std::vector<std::string> m_passFeedbackKeys; // "PassFeedback0", "PassFeedback1", etc.
    std::vector<std::string> m_historyKeys;      // "OriginalHistory1", "OriginalHistory2", etc.
    std::map<std::string, winrt::com_ptr<ID3D11ShaderResourceView>> m_presetTextures;
    std::map<std::string, float4>                                   m_textureSizes;
    std::vector<ShaderPass>                                         m_shaderPasses;

    POINT      m_monitorOffset {0, 0};
    HWND       m_outputWindow {0};
    HWND       m_captureWindow {0};
    bool       m_clone {false};
    bool       m_image {false};
    bool       m_flipMode {false};
    bool       m_allowTearing {false};
    bool       m_useHDR {false};
    int        m_frameCounter {0};
    int        m_logicalFrameCounter {0};
    ULONGLONG  m_startTicks {0};
    int        m_renderCounter {0};
    int        m_prevRenderCounter {0};
    ULONGLONG  m_prevRenderTicks {0};
    ULONGLONG  m_prevTicks {0};
    ULONGLONG  m_prevFrameTicks {0};
    int        m_prevInputFrameNo {0};
    int        m_prevLogicalFrameNo {0};
    float      m_fps {0};
    bool       m_requiresFeedback {false};
    int        m_requiresHistory {0};
    std::mutex m_mutex {};
    int        m_boxX {0};
    int        m_boxY {0};

    CursorEmulator&                                   m_cursorEmulator;
    PassthroughPresetDef                              m_passthroughDef;
    PreprocessShaderDef                               m_preprocessShaderDef;
    PresetDef                                         m_preprocessPresetDef;
    Preset                                            m_preprocessPreset;
    Shader                                            m_preprocessShader;
    ShaderPass                                        m_preprocessPass;
    std::unique_ptr<Preset>                           m_shaderPreset {nullptr};
    std::unique_ptr<Preset>                           m_newShaderPreset {nullptr};
    std::vector<std::tuple<int, std::string, double>> m_newParams;

    // Thread-safe atomic variables (replaced volatile for proper memory ordering)
    std::atomic<int>   m_frameSkip {0};
    std::atomic<bool>  m_running {false};
    std::atomic<float> m_inputScaleW {3.0f};
    std::atomic<float> m_inputScaleH {3.0f};
    std::atomic<bool>  m_inputRescaled {false};
    std::atomic<float> m_outputScaleW {1.0f};
    std::atomic<float> m_outputScaleH {1.0f};
    std::atomic<bool>  m_outputRescaled {false};
    std::atomic<bool>  m_flipHorizontal {false};
    std::atomic<bool>  m_flipVertical {false};
    std::atomic<bool>  m_lockedAreaUpdated {false};
    std::atomic<bool>  m_freeScale {false};
    std::atomic<bool>  m_croppedAreaUpdated {false};
    std::atomic<bool>  m_vertical {false};
    std::atomic<bool>  m_verticalUpdated {false};

    // RECT requires mutex protection (not trivially copyable for atomic)
    struct {
        RECT value {0, 0, 0, 0};
        std::mutex mutex;
    } m_lockedArea;

    struct {
        RECT value {0, 0, 0, 0};
        std::mutex mutex;
    } m_croppedArea;
};
