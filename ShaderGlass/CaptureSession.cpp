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

CaptureSession::CaptureSession(winrt::com_ptr<ID3D11Device>      captureDevice,
                               winrt::com_ptr<ID3D11Device>      renderDevice,
                               winrt::GraphicsCaptureItem const& item,
                               winrt::DirectXPixelFormat         pixelFormat,
                               ShaderGlass&                      shaderGlass,
                               bool                              maxCaptureRate,
                               HANDLE                            frameEvent) :
    m_device {nullptr}, m_item {item}, m_pixelFormat {pixelFormat}, m_shaderGlass {shaderGlass}, m_textureBridge {captureDevice, renderDevice}, m_frameEvent(frameEvent)
{
    auto dxgiDevice = captureDevice.as<IDXGIDevice>();
    auto hr = dxgiDevice->SetGPUThreadPriority(7);
    assert(SUCCEEDED(hr));
    m_device        = CreateDirect3DDevice(dxgiDevice.get());

    m_contentSize = m_item.Size();
    //m_framePool   = winrt::Direct3D11CaptureFramePool::CreateFreeThreaded(m_device, pixelFormat, 2, m_contentSize);
    m_framePool = winrt::Direct3D11CaptureFramePool::Create(m_device, pixelFormat, 4, m_contentSize);
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
            const auto minInterval = maxCaptureRate ? std::chrono::milliseconds(1) : std::chrono::milliseconds(15);
            m_session.MinUpdateInterval(winrt::Windows::Foundation::TimeSpan(minInterval));
            m_session.MinUpdateInterval(winrt::Windows::Foundation::TimeSpan(0));
        }
        catch(...)
        { }
    }

    Reset();
    //m_framePool.FrameArrived({this, &CaptureSession::OnFrameArrived});
    m_session.StartCapture();

    WINRT_ASSERT(m_session != nullptr);
}

CaptureSession::CaptureSession(winrt::com_ptr<ID3D11Texture2D> inputImage, ShaderGlass& shaderGlass, HANDLE frameEvent) :
    m_device {nullptr}, m_inputImage {inputImage}, m_shaderGlass {shaderGlass}, m_textureBridge {nullptr, nullptr}, m_frameEvent {frameEvent}
{
    Reset();
    ProcessInput();
}

void CaptureSession::Reset()
{
    m_numInputFrames  = 0;
    m_prevInputFrames = 0;
    m_fps             = 0;
    m_prevTicks       = GetTicks();
}

void CaptureSession::UpdateCursor(bool captureCursor)
{
    if(m_session && CanUpdateCursor())
        m_session.IsCursorCaptureEnabled(captureCursor);
}

bool CaptureSession::OnFrameArrived(winrt::Direct3D11CaptureFramePool const& sender, winrt::IInspectable const&)
{
    if(!m_session)
        return false;

    auto frame = sender.TryGetNextFrame();
    if(!frame)
        return false;
    auto inputFrame = GetDXGIInterfaceFromObject<ID3D11Texture2D>(frame.Surface());

    auto contentSize = frame.ContentSize();
    auto resized     = false;
    if(contentSize.Width != m_contentSize.Width || contentSize.Height != m_contentSize.Height)
    {
        m_contentSize.Width  = contentSize.Width;
        m_contentSize.Height = contentSize.Height;
        m_framePool.Recreate(m_device, m_pixelFormat, 4, m_contentSize);
        resized = true;
    }

    m_textureBridge.PutInputFrame(inputFrame, resized);

    //SetEvent(m_frameEvent);
    OnInputFrame();

    return true;
}

void CaptureSession::OnInputFrame()
{
    m_frameTicks = GetTicks();
    m_numInputFrames++;
    if(m_frameTicks - m_prevTicks > TICKS_PER_SEC)
    {
        auto deltaTicks   = m_frameTicks - m_prevTicks;
        auto deltaFrames  = m_numInputFrames - m_prevInputFrames;
        m_fps             = deltaFrames * (float)TICKS_PER_SEC / deltaTicks;
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
        auto arr = false;
        do
        {
            arr = OnFrameArrived(m_framePool, nullptr);
        } while(arr);
        m_shaderGlass.Process(m_textureBridge.GetInputFrame(), m_frameTicks, m_numInputFrames);
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

TextureBridge::TextureBridge(winrt::com_ptr<ID3D11Device> captureDevice, winrt::com_ptr<ID3D11Device> renderDevice) : m_captureDevice {captureDevice}, m_renderDevice {renderDevice}
{
    if(m_captureDevice)
    {
        m_captureDevice->GetImmediateContext(m_captureContext.put());
    }
    if(m_renderDevice)
    {
        m_renderDevice->GetImmediateContext(m_renderContext.put());
    }
}

TextureBridge::~TextureBridge()
{
    std::unique_lock lock(m_inputMutex);

    if(m_inputData)
    {
        free(m_inputData);
        m_inputData = nullptr;
    }
}

void TextureBridge::PutInputFrame(winrt::com_ptr<ID3D11Texture2D> inputFrame, bool resized)
{
    if(m_renderDevice)
    {
        // copy to staging texture and then memory buffer
        HRESULT              hr;
        D3D11_TEXTURE2D_DESC sharedDesc = {};
        inputFrame->GetDesc(&sharedDesc);

        if(!m_sharedFrame || resized)
        {
            sharedDesc.Usage          = D3D11_USAGE_STAGING;
            sharedDesc.BindFlags      = 0;
            sharedDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
            sharedDesc.MiscFlags      = 0;
            hr                        = m_captureDevice->CreateTexture2D(&sharedDesc, nullptr, m_sharedFrame.put());
            assert(SUCCEEDED(hr));
        }

        m_captureContext->CopyResource(m_sharedFrame.get(), inputFrame.get());

        D3D11_MAPPED_SUBRESOURCE sourceResource;
        hr = m_captureContext->Map(m_sharedFrame.get(), 0, D3D11_MAP_READ, 0, &sourceResource);
        assert(SUCCEEDED(hr));
        if(sourceResource.DepthPitch)
        {
            std::unique_lock lock(m_inputMutex);

            if(resized || m_inputSize == 0)
            {
                memcpy(&m_inputFrameDesc, &sharedDesc, sizeof(D3D11_TEXTURE2D_DESC));
                m_inputResized = true;
            }

            if(sourceResource.DepthPitch != m_inputSize || !m_inputData)
            {
                if(m_inputData)
                    free(m_inputData);

                m_inputSize = sourceResource.DepthPitch;
                m_inputData = malloc(m_inputSize);
                assert(m_inputData);
            }
            memcpy(m_inputData, sourceResource.pData, sourceResource.DepthPitch);
            m_inputUpdated = true;
        }
        m_captureContext->Unmap(m_sharedFrame.get(), 0);
    }
    else
    {
        HRESULT              hr;
        D3D11_TEXTURE2D_DESC sharedDesc = {};
        inputFrame->GetDesc(&sharedDesc);

        winrt::com_ptr<ID3D11Texture2D> queuedFrame;
        hr = m_captureDevice->CreateTexture2D(&sharedDesc, nullptr, queuedFrame.put());
        assert(SUCCEEDED(hr));

        m_captureContext->CopyResource(queuedFrame.get(), inputFrame.get());

        m_inputFrame = queuedFrame;
    }
}

winrt::com_ptr<ID3D11Texture2D> TextureBridge::GetInputFrame()
{
    if(m_renderDevice && m_inputUpdated)
    {
        // copy from local buffer
        HRESULT hr;
        if(!m_inputFrame || m_inputResized)
        {
            D3D11_TEXTURE2D_DESC inputDesc = {};
            memcpy(&inputDesc, &m_inputFrameDesc, sizeof(D3D11_TEXTURE2D_DESC));

            inputDesc.Usage          = D3D11_USAGE_DYNAMIC;
            inputDesc.BindFlags      = D3D11_BIND_SHADER_RESOURCE;
            inputDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            inputDesc.MiscFlags      = 0;
            hr                       = m_renderDevice->CreateTexture2D(&inputDesc, nullptr, m_inputFrame.put());
            assert(SUCCEEDED(hr));
            m_inputResized = false;
        }

        {
            std::unique_lock lock(m_inputMutex);
            if(m_inputData && m_inputSize)
            {
                D3D11_MAPPED_SUBRESOURCE destinationResource;
                hr = m_renderContext->Map(m_inputFrame.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &destinationResource);
                assert(SUCCEEDED(hr));
                assert(m_inputSize == destinationResource.DepthPitch);
                memcpy(destinationResource.pData, m_inputData, destinationResource.DepthPitch);
                m_renderContext->Unmap(m_inputFrame.get(), 0);
            }
            m_inputUpdated = false;
        }
    }
    return m_inputFrame;
}
