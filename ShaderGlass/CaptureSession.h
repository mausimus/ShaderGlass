/*
ShaderGlass: shader effect overlay
Copyright (C) 2021-2025 mausimus (mausimus.net)
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0
*/

#pragma once

#include "ShaderGlass.h"
#include "CaptureLib.h"

class CaptureSession
{
public:
    CaptureSession(winrt::com_ptr<ID3D11Device>                                  d3dDevice,
                   winrt::Windows::Graphics::Capture::GraphicsCaptureItem const& item,
                   bool                                                          windowInput,
                   HWND                                                          outputWindow,
                   winrt::Windows::Graphics::DirectX::DirectXPixelFormat         pixelFormat,
                   ShaderGlass&                                                  shaderGlass,
                   bool                                                          maxCaptureRate,
                   HANDLE                                                        frameEvent);

    CaptureSession(winrt::com_ptr<ID3D11Texture2D> inputImage, ShaderGlass& shaderGlass, HANDLE frameEvent);

    void OnFrameArrived(winrt::Windows::Graphics::Capture::Direct3D11CaptureFramePool const& sender, winrt::Windows::Foundation::IInspectable const& args);

    void Stop();

    void UpdateCursor(bool captureCursor);

    void OnInputFrame();

    void ProcessInput();

    void GetContentSize(LONG& width, LONG& height);

    float FPS()
    {
        return m_fps;
    }

    void OnCaptureLibArrived(UINT width, UINT height);

private:
    void Reset();

    winrt::Windows::Graphics::Capture::GraphicsCaptureItem         m_item {nullptr};
    winrt::Windows::Graphics::Capture::Direct3D11CaptureFramePool  m_framePool {nullptr};
    winrt::Windows::Graphics::Capture::GraphicsCaptureSession      m_session {nullptr};
    winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice m_device {nullptr};
    winrt::com_ptr<ID3D11Texture2D>                                m_inputImage {nullptr};
    winrt::com_ptr<ID3D11Texture2D>                                m_inputFrame {nullptr};
    winrt::com_ptr<ID3D11Device>                                   m_d3dDevice {nullptr};
    winrt::Windows::Graphics::DirectX::DirectXPixelFormat          m_pixelFormat {0};
    winrt::Windows::Graphics::SizeInt32                            m_contentSize {0, 0};
    ULONGLONG                                                      m_frameTicks {0};
    float                                                          m_fps {0};
    int                                                            m_numInputFrames {0};
    ULONGLONG                                                      m_prevTicks {0};
    int                                                            m_prevInputFrames {0};
    HANDLE                                                         m_frameEvent {nullptr};
    HWND                                                           m_outputWindow {nullptr};
    bool                                                           m_notifySize {false};
    ShaderGlass&                                                   m_shaderGlass;
    CaptureLib                                                     m_captureLib;
};
