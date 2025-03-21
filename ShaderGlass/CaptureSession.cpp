#include "pch.h"
#include "CaptureSession.h"
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
                               HANDLE                            frameEvent) : m_device {device}, m_item {item}, m_shaderGlass {shaderGlass}, m_frameEvent(frameEvent)
{
    m_framePool = winrt::Direct3D11CaptureFramePool::CreateFreeThreaded(m_device, pixelFormat, 2, m_item.Size());
    m_session   = m_framePool.CreateCaptureSession(m_item);

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
        }
        catch(...)
        { }
    }

    m_numInputFrames  = 0;
    m_prevInputFrames = 0;
    m_fps             = 0;
    m_prevTicks       = GetTickCount64();
    m_framePool.FrameArrived({this, &CaptureSession::OnFrameArrived});
    m_session.StartCapture();

    WINRT_ASSERT(m_session != nullptr);
}

CaptureSession::CaptureSession(winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice const& device,
                               winrt::com_ptr<ID3D11Texture2D>                                       inputImage,
                               ShaderGlass&                                                          shaderGlass,
                               HANDLE frameEvent) : m_device {device}, m_inputImage {inputImage}, m_shaderGlass {shaderGlass}, m_frameEvent {frameEvent}
{
    ProcessInput();
}

void CaptureSession::UpdateCursor(bool captureCursor)
{
    if(m_session && winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Graphics.Capture.GraphicsCaptureSession", L"IsCursorCaptureEnabled"))
        m_session.IsCursorCaptureEnabled(captureCursor);
}

void CaptureSession::OnFrameArrived(winrt::Direct3D11CaptureFramePool const& sender, winrt::IInspectable const&)
{
    auto frame   = sender.TryGetNextFrame();
    m_inputFrame = GetDXGIInterfaceFromObject<ID3D11Texture2D>(frame.Surface());
    m_frameTicks = GetTickCount64();
    SetEvent(m_frameEvent);
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
        m_shaderGlass.Process(m_inputImage, 0);
    }
    else
    {
        m_shaderGlass.Process(m_inputFrame, m_frameTicks);
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
