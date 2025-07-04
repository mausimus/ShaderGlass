/*
ShaderGlass: shader effect overlay
Copyright (C) 2021-2025 mausimus (mausimus.net)
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0
*/

#pragma once

#include "ShaderGlass.h"

class CaptureSession
{
public:
    CaptureSession(winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice const& device,
                   winrt::Windows::Graphics::Capture::GraphicsCaptureItem const&         item,
                   winrt::Windows::Graphics::DirectX::DirectXPixelFormat                 pixelFormat,
                   ShaderGlass&                                                          shaderGlass,
                   bool                                                                  maxCaptureRate,
                   HANDLE                                                                frameEvent);

    CaptureSession(winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice const& device,
                   winrt::com_ptr<ID3D11Texture2D>                                       inputImage,
                   ShaderGlass&                                                          shaderGlass,
                   HANDLE                                                                frameEvent);

    void OnFrameArrived(winrt::Windows::Graphics::Capture::Direct3D11CaptureFramePool const& sender, winrt::Windows::Foundation::IInspectable const& args);

    void Stop();

    void UpdateCursor(bool captureCursor);

    void OnInputFrame();

    void ProcessInput();

    float FPS()
    {
        return m_fps;
    }

private:
    void Reset();

    winrt::Windows::Graphics::Capture::GraphicsCaptureItem         m_item {nullptr};
    winrt::Windows::Graphics::Capture::Direct3D11CaptureFramePool  m_framePool {nullptr};
    winrt::Windows::Graphics::Capture::GraphicsCaptureSession      m_session {nullptr};
    winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice m_device {nullptr};
    winrt::com_ptr<ID3D11Texture2D>                                m_inputImage {nullptr};
    winrt::com_ptr<ID3D11Texture2D>                                m_inputFrame {nullptr};
    winrt::Windows::Graphics::DirectX::DirectXPixelFormat          m_pixelFormat {0};
    winrt::Windows::Graphics::SizeInt32                            m_contentSize {0, 0};
    ULONGLONG                                                      m_frameTicks {0};
    float                                                          m_fps {0};
    int                                                            m_numInputFrames {0};
    ULONGLONG                                                      m_prevTicks {0};
    int                                                            m_prevInputFrames {0};
    HANDLE                                                         m_frameEvent {nullptr};
    ShaderGlass&                                                   m_shaderGlass;
};
