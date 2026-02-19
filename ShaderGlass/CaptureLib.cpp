#include "pch.h"
#include "CaptureLib.h"
#include "CaptureSession.h"

bool                              CaptureLib::Enabled           = true;
HMODULE                           CaptureLib::CaptureLibModule  = NULL;
CaptureLib::CaptureLibVersionFunc CaptureLib::CaptureLibVersion = NULL;
CaptureLib::CaptureLibInitFunc    CaptureLib::CaptureLibInit    = NULL;
CaptureLib::CaptureLibStartFunc   CaptureLib::CaptureLibStart   = NULL;
CaptureLib::CaptureLibStopFunc    CaptureLib::CaptureLibStop    = NULL;

static void CaptureLibCallback(void* data, UINT width, UINT height, UINT pitch, void* context)
{
    static_cast<CaptureLib*>(context)->OnFrameArrived(data, width, height, pitch);
}

CaptureLib::CaptureLib(CaptureSession& session) : m_session(session), m_width {0}, m_height {0}, m_active {false} { }

void CaptureLib::Disable()
{
    Enabled = false;
}

bool CaptureLib::Load()
{
    if(!Enabled)
        return false;

    if(CaptureLibModule == NULL)
    {
        DWORD oldMode;
        SetThreadErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX, &oldMode);
        CaptureLibModule = LoadLibrary(L"WineCap.dll");
        if(CaptureLibModule == NULL)
        {
            SetThreadErrorMode(oldMode, NULL);
            throw new std::runtime_error("Unable to load CaptureLib");
        }
        SetThreadErrorMode(oldMode, NULL);

        CaptureLibVersion = (CaptureLibVersionFunc)GetProcAddress(CaptureLibModule, "CaptureLibVersion");
        if(CaptureLibVersion == NULL)
        {
            CaptureLibModule = NULL;
            throw new std::runtime_error("Invalid CaptureLib interface");
        }
        if(CaptureLibVersion() != CaptureLibExpectedVersion)
        {
            CaptureLibModule = NULL;
            throw new std::runtime_error("Unsupported CaptureLib version");
        }

        CaptureLibInit = (CaptureLibInitFunc)GetProcAddress(CaptureLibModule, "CaptureLibInit");
        if(CaptureLibInit == NULL)
        {
            CaptureLibModule = NULL;
            throw new std::runtime_error("Invalid CaptureLib interface");
        }
        if(CaptureLibInit() != S_OK)
        {
            CaptureLibModule = NULL;
            throw new std::runtime_error("Failed to initialize CaptureLib");
        }

        CaptureLibStart = (CaptureLibStartFunc)GetProcAddress(CaptureLibModule, "CaptureLibStart");
        CaptureLibStop  = (CaptureLibStopFunc)GetProcAddress(CaptureLibModule, "CaptureLibStop");
        if(CaptureLibStart == NULL || CaptureLibStop == NULL)
        {
            CaptureLibModule = NULL;
            throw new std::runtime_error("Invalid CaptureLib interface");
        }
    }
    return true;
}

void CaptureLib::Start(winrt::com_ptr<ID3D11Device> device, bool window, bool cursor)
{
    m_device = device;
    m_device->GetImmediateContext(m_context.put());
    m_active = true;
    if(CaptureLibStart(window ? CaptureTypeWindow : CaptureTypeDesktop, cursor ? 1 : 0, CaptureLibCallback, (void*)this) != S_OK)
    {
        throw new std::runtime_error("Unable to start CaptureLib");
    }
}

void CaptureLib::OnFrameArrived(void* data, UINT width, UINT height, UINT pitch)
{
    if(width == 0 || height == 0 || pitch == 0 || data == NULL || !m_active)
        return;

    {
        std::unique_lock lock(m_mutex);

        if(width != m_width || height != m_height || !m_inputFrame)
        {
            // recreate
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
            m_device->CreateTexture2D(&desc, NULL, m_inputFrame.put());
            m_width  = width;
            m_height = height;
        }

        D3D11_MAPPED_SUBRESOURCE mappedResource;
        ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
        m_context->Map(m_inputFrame.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
        if(mappedResource.RowPitch == pitch)
        {
            memcpy(mappedResource.pData, data, pitch * height);
        }
        else
        {
            BYTE* sourcePtr  = (BYTE*)data;
            BYTE* destPtr    = (BYTE*)mappedResource.pData;
            auto  lineLength = width * 4;
            for(int h = 0; h < height; h++)
            {
                memcpy(destPtr, sourcePtr, lineLength);
                sourcePtr += pitch;
                destPtr += mappedResource.RowPitch;
            }
        }
        m_context->Unmap(m_inputFrame.get(), 0);
    }

    m_session.OnCaptureLibArrived(width, height);
}

std::unique_lock<std::mutex> CaptureLib::Lock()
{
    return std::unique_lock(m_mutex);
}

winrt::com_ptr<ID3D11Texture2D> CaptureLib::GetInputFrame()
{
    return m_inputFrame;
}

bool CaptureLib::Active() const
{
    return m_active;
}

void CaptureLib::Stop()
{
    m_active = false;
    CaptureLibStop();
    m_inputFrame = nullptr;
    m_context    = nullptr;
    m_device     = nullptr;
}