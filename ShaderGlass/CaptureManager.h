#pragma once

#include "CaptureSession.h"

struct CaptureOptions
{
    HMONITOR     monitor {0};
    HWND         captureWindow {0};
    HWND         outputWindow {0};
    float        pixelWidth {3.0f};
    float        pixelHeight {3.0f};
    float        aspectRatio {1.0f};
    unsigned     presetNo {0};
    unsigned     frameSkip {0};
    float        outputScale {1};
    bool         flipHorizontal {false};
    bool         flipVertical {false};
    bool         clone {false};
    bool         transparent {false};
    bool         paused {false};
    bool         captureCursor {false};
    std::wstring imageFile {};
    int          imageWidth {0};
    int          imageHeight {0};
    RECT         inputArea {0, 0, 0, 0};
    float        dpiScale {1.0f};
    bool         freeScale {false};
};

class CaptureManager
{
public:
    CaptureManager();

    CaptureOptions m_options;

    const std::vector<std::unique_ptr<PresetDef>>& Presets();
    std::vector<std::tuple<int, ShaderParam*>>     Params();

    bool Initialize();
    bool IsActive();
    bool StartSession();
    void StopSession();
    void Debug();
    int AddPreset(PresetDef* preset);
    void UpdatePixelSize();
    void UpdateOutputSize();
    void UpdateOutputFlip();
    void UpdateShaderPreset();
    void UpdateFrameSkip();
    bool UpdateInput();
    void UpdateCursor();
    void UpdateLockedArea();
    void GrabOutput();
    void UpdateParams();
    void ResetParams();
    void SetParams(const std::vector<std::tuple<int, std::string, double>>& params);
    void RememberLastPreset();
    void SetLastPreset(unsigned presetNo);
    void ForgetLastPreset();
    void SaveOutput(LPWSTR fileName);
    void ProcessFrame();
    void ThreadFunc();
    void Exit();
    float FPS();
    int FindByName(const char* presetName);

private:
    volatile bool                                     m_active {false};
    winrt::com_ptr<ID3D11Device>                      m_d3dDevice {nullptr};
    winrt::com_ptr<ID3D11DeviceContext>               m_context {nullptr};
    winrt::com_ptr<ID3D11Debug>                       m_debug {nullptr};
    winrt::com_ptr<ID3D11Texture2D>                   m_outputTexture {nullptr};
    std::unique_ptr<CaptureSession>                   m_session {nullptr};
    std::unique_ptr<ShaderGlass>                      m_shaderGlass {nullptr};
    std::vector<std::unique_ptr<PresetDef>>           m_presetList;
    std::vector<std::tuple<int, std::string, double>> m_queuedParams;
    std::vector<std::tuple<int, std::string, double>> m_lastParams;
    unsigned int                                      m_lastPreset;
};
