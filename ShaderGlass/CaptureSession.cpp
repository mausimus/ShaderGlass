/*
ShaderGlass: shader effect overlay
Copyright (C) 2021-2025 mausimus (mausimus.net)
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0
*/

#include "pch.h"
#include "CaptureSession.h"
#include "Helpers.h"
#include "Options.h"

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

CaptureSession::CaptureSession(winrt::com_ptr<ID3D11Device>      d3dDevice,
                               winrt::GraphicsCaptureItem const& item,
                               bool                              windowInput,
                               HWND                              outputWindow,
                               winrt::DirectXPixelFormat         pixelFormat,
                               ShaderGlass&                      shaderGlass,
                               bool                              maxCaptureRate,
                               HANDLE                            frameEvent) :
    m_d3dDevice {d3dDevice}, m_item {item}, m_pixelFormat {pixelFormat}, m_shaderGlass {shaderGlass}, m_frameEvent(frameEvent), m_outputWindow(outputWindow), m_captureLib(*this)
{
    if(HasCaptureAPI())
    {
        auto dxgiDevice = m_d3dDevice.as<IDXGIDevice>();
        m_device        = CreateDirect3DDevice(dxgiDevice.get());

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
    else if(HasCaptureLib())
    {
        m_contentSize.Width = 0;
        m_contentSize.Height = 0;
        m_notifySize = true;
        m_captureLib.Start(d3dDevice, windowInput, true);
    }
}

CaptureSession::CaptureSession(winrt::com_ptr<ID3D11Texture2D> inputImage, ShaderGlass& shaderGlass, HANDLE frameEvent) :
    m_d3dDevice(nullptr), m_inputImage {inputImage}, m_device(nullptr), m_shaderGlass {shaderGlass}, m_frameEvent {frameEvent}, m_captureLib(*this)
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
    if(HasCaptureAPI())
    {
        if(m_session && CanUpdateCursor())
            m_session.IsCursorCaptureEnabled(captureCursor);
    }
}

void CaptureSession::OnFrameArrived(winrt::Direct3D11CaptureFramePool const& sender, winrt::IInspectable const&)
{
    auto frame   = sender.TryGetNextFrame();
    m_inputFrame = GetDXGIInterfaceFromObject<ID3D11Texture2D>(frame.Surface());

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

void CaptureSession::OnCaptureLibArrived(UINT width, UINT height)
{
    auto notifySize      = m_notifySize && m_contentSize.Width == 0 && m_contentSize.Height == 0;
    m_contentSize.Width  = width;
    m_contentSize.Height = height;
    SetEvent(m_frameEvent);
    OnInputFrame();
    if(notifySize)
    {
        m_notifySize = 0;
        PostMessage(m_outputWindow, WM_USER_FIRST_FRAME, 0, 0);
    }
}

void CaptureSession::OnInputFrame()
{
    m_frameTicks = GetTickCount64();
    m_numInputFrames++;
    if(m_frameTicks - m_prevTicks > 1000)
    {
        auto deltaTicks   = m_frameTicks - m_prevTicks;
        auto deltaFrames  = m_numInputFrames - m_prevInputFrames;
        m_fps             = deltaFrames * 1000.0f / deltaTicks;
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
    else if(HasCaptureAPI())
    {
        m_shaderGlass.Process(m_inputFrame, m_frameTicks, m_numInputFrames);
    }
    else if(m_captureLib.Active())
    {
        auto lock = m_captureLib.Lock();
        m_shaderGlass.Process(m_captureLib.GetInputFrame(), m_frameTicks, m_numInputFrames);
    }
}

void CaptureSession::GetContentSize(LONG& width, LONG& height)
{
    width  = m_contentSize.Width;
    height = m_contentSize.Height;
}

void CaptureSession::Stop()
{
    if(HasCaptureAPI())
    {
        if(m_session)
            m_session.Close();

        if(m_framePool)
            m_framePool.Close();
    }
    if(m_captureLib.Active())
    {
        m_captureLib.Stop();
    }

    m_framePool = nullptr;
    m_session   = nullptr;
    m_item      = nullptr;
    m_d3dDevice = nullptr;
}
