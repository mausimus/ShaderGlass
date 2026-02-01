/*
ShaderGlass: shader effect overlay
Copyright (C) 2021-2025 mausimus (mausimus.net)
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0
*/

#include "pch.h"
#include "CaptureSession.h"
#include "Helpers.h"

#include "Util/direct3d11.interop.h"

#define USE_CAPTURELIB

extern "C"
{
#include "CaptureLib.h"
}

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

static void CaptureLibCallback(void* data, UINT width, UINT height, void* context)
{
    static_cast<CaptureSession*>(context)->OnCaptureLibArrived(data, width, height);
}

CaptureSession::CaptureSession(winrt::com_ptr<ID3D11Device>      d3dDevice,
                               winrt::GraphicsCaptureItem const& item,
                               winrt::DirectXPixelFormat         pixelFormat,
                               ShaderGlass&                      shaderGlass,
                               bool                              maxCaptureRate,
                               HANDLE frameEvent) : m_d3dDevice {d3dDevice}, m_item {item}, m_pixelFormat {pixelFormat}, m_shaderGlass {shaderGlass}, m_frameEvent(frameEvent)
{
    if(UseCaptureLib())
    {
        m_d3dDevice->GetImmediateContext(m_context.put());
        CaptureLibStart(CaptureLibCallback, (void*)this);
    }
    else
    {
        if(HasCaptureAPI())
        {
            auto dxgiDevice = m_d3dDevice.as<IDXGIDevice>();
            m_device        = CreateDirect3DDevice(dxgiDevice.get());
        }
        else
        {
            m_device = nullptr;
        }

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
}

CaptureSession::CaptureSession(winrt::com_ptr<ID3D11Texture2D> inputImage, ShaderGlass& shaderGlass, HANDLE frameEvent) :
    m_d3dDevice(nullptr), m_inputImage {inputImage}, m_device(nullptr), m_shaderGlass {shaderGlass}, m_frameEvent {frameEvent}
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
#ifndef USE_CAPTURELIB
    if(m_session && CanUpdateCursor())
        m_session.IsCursorCaptureEnabled(captureCursor);
#endif
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

void CaptureSession::OnCaptureLibArrived(void* data, UINT width, UINT height)
{
    if(width == 0 || height == 0 || data == NULL)
        return;

    {
        std::unique_lock lock(m_mutex);

        if(width != m_contentSize.Width || height != m_contentSize.Height || m_inputFrames.empty())
        {
            // recreate
            m_inputFrames.clear();
            for(int i = 0; i < 1; i++)
            {
                D3D11_TEXTURE2D_DESC desc {};
                desc.Usage              = D3D11_USAGE_DYNAMIC;
                desc.BindFlags          = D3D11_BIND_SHADER_RESOURCE;
                desc.CPUAccessFlags     = D3D11_CPU_ACCESS_WRITE;
                desc.MipLevels          = 1;
                desc.SampleDesc.Count   = 1;
                desc.SampleDesc.Quality = 0;
                desc.ArraySize          = 1;
                desc.MiscFlags          = 0;
                desc.Format             = DXGI_FORMAT_B8G8R8A8_UNORM;
                desc.Width              = width;
                desc.Height             = height;
                winrt::com_ptr<ID3D11Texture2D> texture;
                assert(SUCCEEDED(m_d3dDevice->CreateTexture2D(&desc, NULL, texture.put())));
                m_inputFrames.push_back(texture);
            }
            m_nextInput          = 0;
            m_contentSize.Width  = width;
            m_contentSize.Height = height;
        }

        D3D11_MAPPED_SUBRESOURCE mappedResource;
        ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
        auto input = m_inputFrames[m_nextInput].get();
        m_context->Map(input, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
        memcpy(mappedResource.pData, data, width * height * 4);
        m_context->Unmap(input, 0);
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
        m_fps             = deltaFrames * 1000.0f / deltaTicks;
        m_prevInputFrames = m_numInputFrames;
        m_prevTicks       = m_frameTicks;
    }
}

bool CaptureSession::UseCaptureLib()
{
#ifdef USE_CAPTURELIB
    return true;
#else
    return false;
#endif
}

void CaptureSession::ProcessInput()
{
    if(m_inputImage.get())
    {
        m_shaderGlass.Process(m_inputImage, m_frameTicks, m_numInputFrames);
    }
    else if(UseCaptureLib() && m_inputFrames.size())
    {
        std::unique_lock lock(m_mutex);

        auto nextInput = m_nextInput;
        m_nextInput++;
        m_nextInput %= m_inputFrames.size();
        m_shaderGlass.Process(m_inputFrames[nextInput], m_frameTicks, m_numInputFrames);
    }
    else
    {
        m_shaderGlass.Process(m_inputFrame, m_frameTicks, m_numInputFrames);
    }
}

void CaptureSession::Stop()
{
    if(UseCaptureLib())
    {
        CaptureLibStop();
        m_inputFrames.clear();
    }
    else
    {
        if(m_session)
            m_session.Close();

        if(m_framePool)
            m_framePool.Close();
    }

    m_framePool = nullptr;
    m_session   = nullptr;
    m_item      = nullptr;
    m_d3dDevice = nullptr;
    m_context   = nullptr;
}
