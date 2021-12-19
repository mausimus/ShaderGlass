#include "pch.h"
#include "CaptureSession.h"

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

CaptureSession::CaptureSession(winrt::IDirect3DDevice const& device, winrt::GraphicsCaptureItem const& item, winrt::DirectXPixelFormat pixelFormat, ShaderGlass& shaderGlass) :
    m_device {device}, m_item {item}, m_shaderGlass {shaderGlass}
{
    m_framePool = winrt::Direct3D11CaptureFramePool::CreateFreeThreaded(m_device, pixelFormat, 2, m_item.Size());
    m_session   = m_framePool.CreateCaptureSession(m_item);

    // try to disable yellow border
    if(winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Graphics.Capture.GraphicsCaptureSession", L"IsBorderRequired"))
    {
        try
        {
            m_session.IsBorderRequired(false);
        }
        catch(...)
        { }
    }

    m_framePool.FrameArrived({this, &CaptureSession::OnFrameArrived});
    m_session.StartCapture();

    WINRT_ASSERT(m_session != nullptr);
}

CaptureSession::CaptureSession(winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice const& device, winrt::com_ptr<ID3D11Texture2D> inputImage, ShaderGlass& shaderGlass) :
    m_device {device}, m_inputImage {inputImage}, m_shaderGlass {shaderGlass}
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
    auto texture = GetDXGIInterfaceFromObject<ID3D11Texture2D>(frame.Surface());
    m_shaderGlass.Process(texture);
}

void CaptureSession::ProcessInput()
{
    m_shaderGlass.Process(m_inputImage);
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
