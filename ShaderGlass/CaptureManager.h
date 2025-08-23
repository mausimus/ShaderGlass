/*
ShaderGlass: shader effect overlay
Copyright (C) 2021-2025 mausimus (mausimus.net)
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0
*/

#pragma once

#include "CaptureSession.h"
#include "ShaderCache.h"
#include "DeviceCapture.h"
#include "CursorEmulator.h"

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
    int          deviceFormatNo {0};
    RECT         inputArea {0, 0, 0, 0};
    float        dpiScale {1.0f};
    bool         freeScale {false};
    bool         flipMode {false};
    bool         allowTearing {false};
    bool         maxCaptureRate {false};
    bool         useHDR {false};
    RECT         croppedArea {0, 0, 0, 0};
    bool         vertical {false};
    unsigned     subFrames {0};
    bool         syncSubFrame {true};
    bool         internalVSync {false};
};

class CaptureManager
{
public:
    CaptureManager(HINSTANCE instance);

    CaptureOptions m_options;
    bool           m_defaultAdapter {true};

    const std::vector<std::unique_ptr<PresetDef>>& Presets();
    std::vector<std::tuple<int, ShaderParam*>>     Params();
    const ShaderCache&                             Cache();
    const std::vector<CaptureDevice>&              CaptureDevices();
    const std::vector<GraphicsAdapter>&            GraphicsAdapters();
    void                                           ShowCursor();
    void                                           HideCursor();

    bool  Initialize();
    bool  IsActive();
    bool  StartSession();
    void  StopSession();
    void  Debug();
    int   AddPreset(PresetDef* preset);
    void  UpdatePixelSize();
    void  UpdateOutputSize();
    void  UpdateOutputFlip();
    void  UpdateSyncSubFrame();
    void  UpdateInternalVSync();
    void  UpdateShaderPreset();
    void  UpdateFrameSkip();
    bool  UpdateInput();
    void  UpdateCursor();
    void  UpdateLockedArea();
    void  UpdateCroppedArea();
    void  UpdateVertical();
    void  UpdateSubFrames();
    void  GrabOutput();
    void  UpdateParams();
    void  ResetParams();
    void  SetParams(const std::vector<std::tuple<int, std::string, double>>& params);
    float GetDefaultValue(ShaderParam* p);
    void  RememberLastPreset();
    void  SetLastPreset(unsigned presetNo);
    void  ForgetLastPreset();
    void  SaveOutput(LPWSTR fileName);
    void  ProcessFrame();
    void  ThreadFunc();
    void  Exit();
    float InFPS();
    float OutFPS();
    int   FindByName(const char* presetName);
    bool  FindDeviceFormat(int deviceFormatNo, std::vector<CaptureDevice>::const_iterator& device, std::vector<CaptureFormat>::const_iterator& format);
    void  SetGraphicsAdapters(int captureNo, int renderNo, LUID& captureId, LUID& renderId);
    void  SetGraphicsAdapters(LUID captureId, LUID renderId);
    LUID  GetAdapterLuid(winrt::com_ptr<ID3D11Device> device);

private:
    volatile bool                                     m_active {false};
    winrt::com_ptr<ID3D11Device>                      m_captureDevice {nullptr};
    winrt::com_ptr<ID3D11Device>                      m_renderDevice {nullptr};
    winrt::com_ptr<ID3D11DeviceContext>               m_renderContext {nullptr};
    winrt::com_ptr<ID3D11Debug>                       m_debug {nullptr};
    winrt::com_ptr<ID3D11Texture2D>                   m_outputTexture {nullptr};
    std::unique_ptr<CaptureSession>                   m_session {nullptr};
    std::unique_ptr<ShaderGlass>                      m_shaderGlass {nullptr};
    std::vector<std::unique_ptr<PresetDef>>           m_presetList;
    std::vector<std::tuple<int, std::string, double>> m_queuedParams;
    std::vector<std::tuple<int, std::string, double>> m_lastParams;
    std::vector<CaptureDevice>                        m_captureDevices;
    std::vector<GraphicsAdapter>                      m_graphicsAdapters;
    ShaderCache                                       m_shaderCache;
    DeviceCapture                                     m_deviceCapture;
    CursorEmulator                                    m_cursorEmulator;
    HANDLE                                            m_frameEvent {nullptr};
    HANDLE                                            m_thread {0};
    HINSTANCE                                         m_instance {0};
    unsigned int                                      m_lastPreset;
};
