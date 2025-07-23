/*
ShaderGlass: shader effect overlay
Copyright (C) 2021-2025 mausimus (mausimus.net)
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0
*/

#include "pch.h"
#include "CaptureManager.h"
#include "ShaderList.h"
#include "Helpers.h"

#include "Util/capture.desktop.interop.h"
#include "Util/direct3d11.interop.h"
#include "Util/d3dHelpers.h"

#include <wincodec.h>
#include "WIC\ScreenGrab11.h"
#include "WIC\WICTextureLoader11.h"

using namespace std;
using namespace util;
using namespace util::uwp;

CaptureManager::CaptureManager(HINSTANCE instance) : m_options(), m_deviceName(L"Default"), m_lastPreset(-1), m_instance {instance} { }

bool CaptureManager::Initialize()
{
    m_presetList.push_back(make_unique<PassthroughPresetDef>());
    m_presetList.insert(m_presetList.end(), RetroArchPresetList.begin(), RetroArchPresetList.end());
    m_frameEvent = CreateEvent(NULL, FALSE, FALSE, L"FrameEvent");
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

const ShaderCache& CaptureManager::Cache()
{
    if(m_shaderCache.empty())
    {
        const auto& raShaders = RetroArchCachedShaders();
        m_shaderCache.m_cachedShaders.insert(m_shaderCache.m_cachedShaders.begin(), raShaders.begin(), raShaders.end());
    }

    return m_shaderCache;
}

void CaptureManager::HideCursor()
{
    m_cursorEmulator.HideCursors();
}

void CaptureManager::ShowCursor()
{
    m_cursorEmulator.ShowCursors();
}

const std::vector<CaptureDevice>& CaptureManager::CaptureDevices()
{
    return m_captureDevices;
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
    auto device     = HasCaptureAPI() ? CreateDirect3DDevice(dxgiDevice.get()) : nullptr;

    // get GPU name
    {
        winrt::com_ptr<IDXGIAdapter> adapter;
        if(SUCCEEDED(dxgiDevice->GetAdapter(adapter.put())))
        {
            DXGI_ADAPTER_DESC desc;
            if(SUCCEEDED(adapter->GetDesc(&desc)))
            {
                m_deviceName = std::wstring(desc.Description);
            }
        }
    }

#ifdef _DEBUG
    m_d3dDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(m_debug.put()));
#endif

    winrt::Windows::Graphics::Capture::GraphicsCaptureItem captureItem {nullptr};

    auto isCaptureAPI = !m_options.imageFile.size() && m_options.deviceFormatNo == 0;
    if(isCaptureAPI)
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

    m_shaderGlass = make_unique<ShaderGlass>(m_cursorEmulator);
    m_shaderGlass->Initialize(m_options.outputWindow,
                              m_options.captureWindow,
                              m_options.monitor,
                              m_options.clone,
                              !m_options.imageFile.empty() || m_options.deviceFormatNo,
                              m_options.flipMode,
                              m_options.allowTearing,
                              m_options.useHDR,
                              m_d3dDevice,
                              m_context);
    UpdatePixelSize();
    UpdateOutputSize();
    UpdateOutputFlip();
    UpdateShaderPreset();
    UpdateFrameSkip();
    UpdateLockedArea();
    UpdateCroppedArea();
    UpdateVertical();

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

        m_session = make_unique<CaptureSession>(device, inputTexture, *m_shaderGlass, m_frameEvent);
        UpdatePixelSize();
    }
    else if(m_options.deviceFormatNo)
    {
        std::vector<CaptureDevice>::const_iterator di;
        std::vector<CaptureFormat>::const_iterator fi;
        if(!FindDeviceFormat(m_options.deviceFormatNo, di, fi))
            return false;

        m_deviceCapture.Start(m_d3dDevice, di->no, fi->no);

        // retrieve input image size
        auto                 inputTexture = m_deviceCapture.m_outputTexture;
        D3D11_TEXTURE2D_DESC desc         = {};
        inputTexture->GetDesc(&desc);
        m_options.imageWidth  = desc.Width;
        m_options.imageHeight = desc.Height;

        m_session = make_unique<CaptureSession>(device, inputTexture, *m_shaderGlass, m_frameEvent);
        UpdatePixelSize();
    }
    else
    {
        winrt::Windows::Graphics::DirectX::DirectXPixelFormat pixelFormat = m_options.useHDR ? winrt::Windows::Graphics::DirectX::DirectXPixelFormat::R16G16B16A16Float
                                                                                             : winrt::Windows::Graphics::DirectX::DirectXPixelFormat::B8G8R8A8UIntNormalized;

        m_session = make_unique<CaptureSession>(device, captureItem, pixelFormat, *m_shaderGlass, m_options.maxCaptureRate, m_frameEvent);
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

float CaptureManager::GetDefaultValue(ShaderParam* p)
{
    if(m_shaderGlass)
    {
        return m_shaderGlass->GetDefaultValue(p);
    }
    return p->defaultValue;
}

void CaptureManager::UpdateCursor()
{
    if(m_options.captureCursor && m_options.transparent)
    {
        m_cursorEmulator.Init(m_d3dDevice, m_instance, m_options.outputWindow);
        m_cursorEmulator.Start();
        if(m_session)
            m_session->UpdateCursor(false);
    }
    else
    {
        m_cursorEmulator.Stop();
        if(m_session)
            m_session->UpdateCursor(m_options.captureCursor);
    }
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
        return (int)m_presetList.size() - 1;
    }
}

bool CaptureManager::IsActive()
{
    return m_session.get();
}

float CaptureManager::OutFPS()
{
    if(m_shaderGlass)
    {
        return m_shaderGlass->FPS();
    }
    return 0.f;
}

float CaptureManager::InFPS()
{
    if(m_session)
    {
        return m_session->FPS();
    }
    return 0.f;
}

void CaptureManager::ProcessFrame()
{
    if(m_session.get())
    {
        if(m_deviceCapture.m_active && m_deviceCapture.Poll())
            m_session->OnInputFrame();
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
        m_cursorEmulator.Stop();

        m_active = false;
        SetEvent(m_frameEvent);

        if(m_deviceCapture.m_active)
            m_deviceCapture.Stop();

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
        if(m_options.vertical)
            m_shaderGlass->SetOutputScale(1.0f / m_options.outputScale, 1.0f / m_options.aspectRatio / m_options.outputScale);
        else
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

void CaptureManager::UpdateCroppedArea()
{
    if(m_shaderGlass)
    {
        m_shaderGlass->SetCroppedArea(m_options.croppedArea);
    }
}

void CaptureManager::UpdateVertical()
{
    if(m_shaderGlass)
    {
        m_shaderGlass->SetVertical(m_options.vertical);
        UpdateOutputSize();
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
        WaitForSingleObject(m_frameEvent, 1);
        ProcessFrame();
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

bool CaptureManager::FindDeviceFormat(int deviceFormatNo, std::vector<CaptureDevice>::const_iterator& device, std::vector<CaptureFormat>::const_iterator& format)
{
    auto        found   = false;
    const auto& devices = CaptureDevices();
    for(auto di = devices.begin(); di != devices.end(); di++)
    {
        for(auto fi = di->formats.begin(); fi != di->formats.end(); fi++)
        {
            if(fi->deviceFormatNo == m_options.deviceFormatNo)
            {
                device = di;
                format = fi;
                return true;
            }
        }
    }

    return false;
}
