#include "pch.h"
#include "CaptureManager.h"
#include "ShaderList.h"

#include "Util/capture.desktop.interop.h"
#include "Util/direct3d11.interop.h"
#include "Util/d3dHelpers.h"

#include <wincodec.h>
#include "WIC\ScreenGrab11.h"
#include "WIC\WICTextureLoader11.h"

using namespace std;
using namespace util;
using namespace util::uwp;

class CaptureManager* CaptureManager::s_this {nullptr};

CaptureManager::CaptureManager() : m_options(), m_timer(0) { }

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

vector<tuple<int, ShaderParam*>> CaptureManager::Params()
{
    if(IsActive())
        return m_shaderGlass->Params();

    return vector<tuple<int, ShaderParam*>>();
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
    if(!m_d3dDevice)
    {
        m_d3dDevice = CreateD3DDevice();
        m_d3dDevice->GetImmediateContext(m_context.put());
    }

    auto dxgiDevice = m_d3dDevice.as<IDXGIDevice>();
    auto device     = CreateDirect3DDevice(dxgiDevice.get());

#ifdef _DEBUG
    m_d3dDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(m_debug.put()));
#endif

    m_shaderGlass = make_unique<ShaderGlass>();
    m_shaderGlass->Initialize(m_options.outputWindow, m_options.captureWindow, m_options.monitor, m_options.clone, !m_options.imageFile.empty(), m_d3dDevice, m_context);
    UpdatePixelSize();
    UpdateOutputSize();
    UpdateOutputFlip();
    UpdateShaderPreset();
    UpdateFrameSkip();
    UpdateLockedArea();

    if(m_options.imageFile.size())
    {
        winrt::com_ptr<ID3D11Texture2D>          inputTexture;
        winrt::com_ptr<ID3D11ShaderResourceView> inputTextureView;
        auto hr = DirectX::CreateWICTextureFromFile(m_d3dDevice.get(), m_options.imageFile.c_str(), (ID3D11Resource**)(inputTexture.put()), inputTextureView.put());
        assert(SUCCEEDED(hr));

        // retrieve input image size
        D3D11_TEXTURE2D_DESC desc = {};
        inputTexture->GetDesc(&desc);
        m_options.imageWidth  = desc.Width;
        m_options.imageHeight = desc.Height;

        m_session = make_unique<CaptureSession>(device, inputTexture, *m_shaderGlass);

        int frameInterval = 1000 / 60; // set to 60fps if we fail to retrieve refresh rate

        DEVMODE devmode;
        devmode.dmSize = sizeof(DEVMODE);
        if(EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devmode) && devmode.dmDisplayFrequency != 0)
        {
            frameInterval = 1000 / devmode.dmDisplayFrequency;
        }
        s_this  = this; // horrible hack
        m_timer = SetTimer(NULL, 0, frameInterval, CaptureManager::TimerFuncProxy);
    }
    else
    {
        auto captureItem = m_options.captureWindow ? CreateCaptureItemForWindow(m_options.captureWindow) : CreateCaptureItemForMonitor(m_options.monitor);

        m_session = make_unique<CaptureSession>(device, captureItem, winrt::Windows::Graphics::DirectX::DirectXPixelFormat::B8G8R8A8UIntNormalized, *m_shaderGlass);
    }
    UpdateCursor();
}

void CaptureManager::SetParams(const std::vector<std::tuple<int, std::string, double>>& params)
{
    m_queuedParams = params;
}

void CaptureManager::UpdateCursor()
{
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

void CaptureManager::ForceProcess()
{
    if(m_session.get())
    {
        m_session->ProcessInput();
    }
}

void CaptureManager::StopSession()
{
    if(m_session.get())
    {
        GrabOutput();

        Exit();
    }
}

void CaptureManager::Exit()
{
    if(m_session.get())
    {
        if(m_timer)
        {
            KillTimer(NULL, m_timer);
            m_timer = 0;
        }

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
        m_shaderGlass->SetInputScale(m_options.pixelWidth * m_options.dpiScale, m_options.pixelHeight * m_options.dpiScale);
    }
}

void CaptureManager::UpdateOutputSize()
{
    if(m_shaderGlass)
    {
        m_shaderGlass->SetOutputScale(1.0f * m_options.aspectRatio / m_options.outputScale, 1.0f / m_options.outputScale);
        m_shaderGlass->SetFreeScale(m_options.freeScale);
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
        m_shaderGlass->SetShaderPreset(m_presetList.at(m_options.presetNo).get(), m_queuedParams);
        m_queuedParams.clear();
    }
}

void CaptureManager::UpdateFrameSkip()
{
    if(m_shaderGlass)
    {
        m_shaderGlass->SetFrameSkip(m_options.frameSkip);
    }
}

void CaptureManager::UpdateLockedArea()
{
    if(m_shaderGlass)
    {
        m_shaderGlass->SetLockedArea(m_options.inputArea);
    }
}

void CaptureManager::GrabOutput()
{
    if(m_shaderGlass)
    {
        m_outputTexture = nullptr;
        m_outputTexture = m_shaderGlass->GrabOutput();
    }
}

void CaptureManager::SaveOutput(LPWSTR fileName)
{
    if(m_outputTexture)
    {
        DirectX::SaveWICTextureToFile(m_context.get(), m_outputTexture.get(), GUID_ContainerFormatPng, fileName, nullptr, nullptr, true);
    }
}

void CaptureManager::UpdateParams()
{
    if(m_shaderGlass)
    {
        m_shaderGlass->UpdateParams();
    }
}

void CaptureManager::ResetParams()
{
    if(m_shaderGlass)
    {
        m_shaderGlass->ResetParams();
    }
}

void CaptureManager::TimerFuncProxy(_In_ HWND hwnd, _In_ UINT param2, _In_ UINT_PTR param3, _In_ DWORD param4)
{
    if(s_this)
    {
        s_this->ForceProcess();
    }
}