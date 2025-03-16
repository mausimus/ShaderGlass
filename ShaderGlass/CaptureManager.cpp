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

CaptureManager::CaptureManager() : m_options(), m_lastPreset(-1) { }

bool CaptureManager::Initialize()
{
    m_presetList.push_back(make_unique<PassthroughPresetDef>());
    m_presetList.insert(m_presetList.end(), RetroArchPresetList.begin(), RetroArchPresetList.end());
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

bool CaptureManager::UpdateInput()
{
    if(IsActive())
    {
        StopSession();
        return StartSession();
    }
    return true;
}

DWORD WINAPI ThreadFuncProxy(LPVOID lpParam)
{
    ((CaptureManager*)lpParam)->ThreadFunc();
    return 0;
}

bool CaptureManager::StartSession()
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

    winrt::Windows::Graphics::Capture::GraphicsCaptureItem captureItem {nullptr};
    if(!m_options.imageFile.size())
    {
        try
        {
            captureItem = m_options.captureWindow ? CreateCaptureItemForWindow(m_options.captureWindow) : CreateCaptureItemForMonitor(m_options.monitor);
        }
        catch(winrt::hresult_error const& error)
        {
            MessageBox(m_options.outputWindow, error.message().c_str(), L"ShaderGlass", MB_OK | MB_ICONERROR);
            return false;
        }
    }

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
        auto                                     hr = DirectX::CreateWICTextureFromFileEx(m_d3dDevice.get(),
                                                      m_options.imageFile.c_str(),
                                                      0,
                                                      D3D11_USAGE_DEFAULT,
                                                      D3D11_BIND_SHADER_RESOURCE,
                                                      0,
                                                      0,
                                                      DirectX::WIC_LOADER_IGNORE_SRGB, // "If the sRGB chunk is found, it is assumed to be gamma 2.2"
                                                      (ID3D11Resource**)(inputTexture.put()),
                                                      inputTextureView.put());
        assert(SUCCEEDED(hr));

        // retrieve input image size
        D3D11_TEXTURE2D_DESC desc = {};
        inputTexture->GetDesc(&desc);
        m_options.imageWidth  = desc.Width;
        m_options.imageHeight = desc.Height;

        m_session = make_unique<CaptureSession>(device, inputTexture, *m_shaderGlass);
        UpdatePixelSize();
    }
    else
    {
        m_session = make_unique<CaptureSession>(device, captureItem, winrt::Windows::Graphics::DirectX::DirectXPixelFormat::B8G8R8A8UIntNormalized, *m_shaderGlass);
    }

    m_active = true;
    CreateThread(NULL, 0, ThreadFuncProxy, this, 0, NULL);

    UpdateCursor();
    return true;
}

void CaptureManager::SetParams(const std::vector<std::tuple<int, std::string, double>>& params)
{
    m_queuedParams = params;
    ForgetLastPreset();
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

int CaptureManager::AddPreset(PresetDef* preset)
{
    preset->MakeDynamic();
    int existing = 0, i = 0;
    for(const auto& p : m_presetList)
    {
        if(p->Name == preset->Name && p->Category == preset->Category)
        {
            existing = i;
            break;
        }
        i++;
    }
    if(existing)
    {
        m_presetList[existing].reset(preset);
        return existing;
    }
    else
    {
        m_presetList.push_back(std::unique_ptr<PresetDef>(preset));
        return m_presetList.size() - 1;
    }
}

bool CaptureManager::IsActive()
{
    return m_session.get();
}

float CaptureManager::FPS()
{
    if(m_shaderGlass)
    {
        return m_shaderGlass->FPS();
    }
    return 0.f;
}

void CaptureManager::ProcessFrame()
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
        RememberLastPreset();

        GrabOutput();

        Exit();
    }
}

void CaptureManager::Exit()
{
    if(m_session.get())
    {
        m_active = false;

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
        // restore params when restarting
        if(m_lastPreset == m_options.presetNo && !m_queuedParams.size() && m_lastParams.size())
        {
            SetParams(m_lastParams);
        }
        m_shaderGlass->SetShaderPreset(m_presetList.at(m_options.presetNo).get(), m_queuedParams);
        m_queuedParams.clear();
        m_lastPreset = m_options.presetNo;
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

void CaptureManager::ThreadFunc()
{
    while(m_active)
    {
        ProcessFrame();
        Sleep(1);
    }
}

void CaptureManager::RememberLastPreset()
{
    if(m_shaderGlass)
    {
        auto params = Params();
        m_lastParams.clear();
        for(const auto& param : params)
        {
            const auto pass        = std::get<0>(param);
            const auto shaderParam = std::get<1>(param);
            m_lastParams.push_back(std::make_tuple(pass, shaderParam->name, shaderParam->currentValue));
        }
    }
}

void CaptureManager::SetLastPreset(unsigned presetNo)
{
    m_lastPreset = presetNo;
}

void CaptureManager::ForgetLastPreset()
{
    m_lastParams.clear();
    m_lastPreset = -1;
}

int CaptureManager::FindByName(const char* presetName)
{
    int p = 0;
    while(p < m_presetList.size())
    {
        if(m_presetList[p]->Name == presetName)
        {
            return p;
        }
        p++;
    }
    return -1;
}
