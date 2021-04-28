#include "pch.h"
#include "CaptureManager.h"
#include "ShaderList.h"

#include "Util/capture.desktop.interop.h"
#include "Util/direct3d11.interop.h"
#include "Util/d3dHelpers.h"

using namespace std;
using namespace util;
using namespace util::uwp;

CaptureManager::CaptureManager() : m_options() { }

bool CaptureManager::Initialize()
{
    m_presetList.push_back(make_unique<PassthroughPresetDef>());
    m_presetList.insert(m_presetList.begin(), RetroArchPresetList.begin(), RetroArchPresetList.end());
    return false;
}

const vector<unique_ptr<PresetDef>>& CaptureManager::Presets()
{
    return m_presetList;
}

void CaptureManager::UpdateInput()
{
    if(IsActive())
    {
        StopSession();
        StartSession();
    }
}

void CaptureManager::StartSession()
{
    m_d3dDevice  = CreateD3DDevice();
    auto dxgiDevice = m_d3dDevice.as<IDXGIDevice>();
    auto device     = CreateDirect3DDevice(dxgiDevice.get());
    auto captureItem =
        m_options.captureWindow ? CreateCaptureItemForWindow(m_options.captureWindow) : CreateCaptureItemForMonitor(m_options.monitor);

#ifdef _DEBUG
    m_d3dDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(m_debug.put()));
#endif

    m_shaderGlass = make_unique<ShaderGlass>();
    m_shaderGlass->Initialize(m_options.outputWindow, m_options.captureWindow, m_options.monitor, m_options.clone, m_d3dDevice);
    UpdatePixelSize();
    UpdateOutputSize();
    UpdateOutputFlip();
    UpdateShaderPreset();
    UpdateFrameSkip();
    m_session = make_unique<CaptureSession>(
        device, captureItem, winrt::Windows::Graphics::DirectX::DirectXPixelFormat::B8G8R8A8UIntNormalized, *m_shaderGlass);
    UpdateCursor();
}

void CaptureManager::UpdateCursor() {
    if(m_session)
        m_session->UpdateCursor(m_options.captureCursor);
}

void CaptureManager::Debug()
{
    if(m_debug)
        m_debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL | D3D11_RLDO_IGNORE_INTERNAL);
}

bool CaptureManager::IsActive()
{
    return m_session.get();
}

void CaptureManager::StopSession()
{
    if(m_session.get())
    {
        m_session->Stop();
        delete m_session.release();

        m_shaderGlass->Stop();
        delete m_shaderGlass.release();

        if(m_debug)
        {
            m_debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL | D3D11_RLDO_IGNORE_INTERNAL);
            m_debug = nullptr;
        }
    }
}

void CaptureManager::UpdatePixelSize()
{
    if(m_shaderGlass)
    {
        m_shaderGlass->SetInputScale(m_options.pixelWidth, m_options.pixelHeight);
    }
}

void CaptureManager::UpdateOutputSize()
{
    if(m_shaderGlass)
    {
        m_shaderGlass->SetOutputScale(1.0f * m_options.aspectRatio / m_options.outputScale, 1.0f / m_options.outputScale);
    }
}

void CaptureManager::UpdateOutputFlip()
{
    if(m_shaderGlass)
    {
        m_shaderGlass->SetOutputFlip(m_options.flipHorizontal, m_options.flipVertical);
    }
}

void CaptureManager::UpdateShaderPreset()
{
    if(m_shaderGlass)
    {
        m_shaderGlass->SetShaderPreset(m_presetList.at(m_options.presetNo).get());
    }
}

void CaptureManager::UpdateFrameSkip()
{
    if(m_shaderGlass)
    {
        m_shaderGlass->SetFrameSkip(m_options.frameSkip);
    }
}
