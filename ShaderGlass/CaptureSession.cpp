/*
ShaderGlass: shader effect overlay
Copyright (C) 2021-2025 mausimus (mausimus.net)
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0
*/

#include "pch.h"
#include "CaptureSession.h"
#include "../ShaderGC/SafeParsing.h"
#include "Helpers.h"

#include "Util/direct3d11.interop.h"

namespace winrt
{
using namespace Windows::Foundation;
using namespace Windows::System;
using namespace Windows::Graphics;
using namespace Windows::Graphics::Capture;
using namespace Windows::Graphics::DirectX;
using namespace Windows::Graphics::DirectX::Direct3D11;
using namespace Windows::Foundation::Numerics;
using namespace Windows::UI;
using namespace Windows::UI::Composition;
} // namespace winrt

CaptureSession::CaptureSession(winrt::IDirect3DDevice const&     device,
                               winrt::GraphicsCaptureItem const& item,
                               winrt::DirectXPixelFormat         pixelFormat,
                               ShaderGlass&                      shaderGlass,
                               bool                              maxCaptureRate,
                               HANDLE frameEvent) : m_device {device}, m_item {item}, m_pixelFormat {pixelFormat}, m_shaderGlass {shaderGlass}, m_frameEvent(frameEvent)
{
    m_contentSize = m_item.Size();
    m_framePool   = winrt::Direct3D11CaptureFramePool::CreateFreeThreaded(m_device, pixelFormat, 2, m_contentSize);
    m_session     = m_framePool.CreateCaptureSession(m_item);

    // try to disable yellow border
    if(CanDisableBorder())
    {
        try
        {
            m_session.IsBorderRequired(false);
        }
        catch(...)
        { }
    }

    if(CanSetCaptureRate())
    {
        try
        {
            // max 250Hz?
            const auto minInterval = maxCaptureRate ? std::chrono::milliseconds(4) : std::chrono::milliseconds(15);
            m_session.MinUpdateInterval(winrt::Windows::Foundation::TimeSpan(minInterval));
            if(maxCaptureRate)
                m_session.MinUpdateInterval(winrt::Windows::Foundation::TimeSpan(0));
        }
        catch(...)
        { }
    }

    Reset();
    m_framePool.FrameArrived({this, &CaptureSession::OnFrameArrived});
    m_session.StartCapture();

    WINRT_ASSERT(m_session != nullptr);
}

CaptureSession::CaptureSession(winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice const& device,
                               winrt::com_ptr<ID3D11Texture2D>                                       inputImage,
                               ShaderGlass&                                                          shaderGlass,
                               HANDLE frameEvent) : m_device {device}, m_inputImage {inputImage}, m_shaderGlass {shaderGlass}, m_frameEvent {frameEvent}
{
    Reset();
    ProcessInput();
}

void CaptureSession::Reset()
{
    m_numInputFrames  = 0;
    m_prevInputFrames = 0;
    m_fps             = 0;
    m_prevTicks       = GetTickCount64();
}

void CaptureSession::UpdateCursor(bool captureCursor)
{
    if(m_session && CanUpdateCursor())
        m_session.IsCursorCaptureEnabled(captureCursor);
}

void CaptureSession::OnFrameArrived(winrt::Direct3D11CaptureFramePool const& sender, winrt::IInspectable const&)
{
    auto frame = sender.TryGetNextFrame();

    // Thread safety: Protect m_inputFrame access with mutex
    {
        std::lock_guard<std::mutex> lock(m_inputFrameMutex);
        m_inputFrame = GetDXGIInterfaceFromObject<ID3D11Texture2D>(frame.Surface());
        m_frameAvailable = true;
    }

    auto contentSize = frame.ContentSize();
    if(contentSize.Width != m_contentSize.Width || contentSize.Height != m_contentSize.Height)
    {
        m_contentSize.Width  = contentSize.Width;
        m_contentSize.Height = contentSize.Height;
        m_framePool.Recreate(m_device, m_pixelFormat, 2, m_contentSize);
    }

    SetEvent(m_frameEvent);
    OnInputFrame();
}

void CaptureSession::OnInputFrame()
{
    m_frameTicks = GetTickCount64();
    m_numInputFrames++;
    if(m_frameTicks - m_prevTicks > 1000)
    {
        auto deltaTicks   = m_frameTicks - m_prevTicks;
        auto deltaFrames  = m_numInputFrames - m_prevInputFrames;
        m_fps             = SafeDivide(deltaFrames * 1000.0f, static_cast<float>(deltaTicks), 0.0f);
        m_prevInputFrames = m_numInputFrames;
        m_prevTicks       = m_frameTicks;
    }
}

void CaptureSession::ProcessInput()
{
    if(m_inputImage.get())
    {
        m_shaderGlass.Process(m_inputImage, m_frameTicks, m_numInputFrames);
    }
    else
    {
        // Thread safety: Protect m_inputFrame access with mutex
        winrt::com_ptr<ID3D11Texture2D> frameToProcess;
        {
            std::lock_guard<std::mutex> lock(m_inputFrameMutex);
            if(m_frameAvailable)
            {
                frameToProcess = m_inputFrame;
                m_frameAvailable = false;
            }
        }

        if(frameToProcess)
        {
            m_shaderGlass.Process(frameToProcess, m_frameTicks, m_numInputFrames);
        }
    }
}

void CaptureSession::Stop()
{
    if(m_session)
        m_session.Close();

    if(m_framePool)
        m_framePool.Close();

    m_framePool = nullptr;
    m_session   = nullptr;
    m_item      = nullptr;
}
