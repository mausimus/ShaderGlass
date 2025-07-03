#pragma once

#include <mfapi.h>
#include <mfidl.h>
#include <Mfreadwrite.h>
#include <mferror.h>

#include "Options.h"

#define MAX_CAPTURE_DEVICES 16U
#define MAX_CAPTURE_FORMATS 256U

class DeviceCapture
{
public:
    DeviceCapture();

    std::vector<CaptureDevice> GetCaptureDevices();
    void                       Start(winrt::com_ptr<ID3D11Device> d3dDevice, int deviceNo, int formatNo);
    void                       Stop();
    bool                       Poll();
    void                       Process(IMFSample* inputSample);

    winrt::com_ptr<ID3D11Texture2D> m_outputTexture;
    bool                            m_active {false};

private:
    class SourceReaderCallback : public IMFSourceReaderCallback
    {
    public:
        SourceReaderCallback(DeviceCapture* capture);

        // IUnknown methods
        STDMETHODIMP QueryInterface(REFIID iid, void** ppv);
        STDMETHODIMP_(ULONG) AddRef();
        STDMETHODIMP_(ULONG) Release();
        // IMFSourceReaderCallback methods
        STDMETHODIMP OnReadSample(HRESULT hrStatus, DWORD dwStreamIndex, DWORD dwStreamFlags, LONGLONG llTimestamp, IMFSample* pSample);
        STDMETHODIMP OnEvent(DWORD, IMFMediaEvent*);
        STDMETHODIMP OnFlush(DWORD);

        volatile bool m_waiting {false};

    private:
        virtual ~SourceReaderCallback() { }

        long             m_nRefCount;
        DeviceCapture*   m_capture;
        CRITICAL_SECTION m_critsec;
    };

    void Init();
    void CreateMediaSource(unsigned deviceNo, unsigned streamNo, unsigned mediaNo);
    void CreateSourceReader();
    void SetMediaType();
    void CreateSampleAllocator(winrt::com_ptr<ID3D11Device>);
    void CreateOutputTexture();

    HRESULT CopyAttribute(IMFAttributes*, IMFAttributes*, REFGUID);

    winrt::com_ptr<IMFMediaSource>            m_mediaSource;
    winrt::com_ptr<IMFSourceReader>           m_sourceReader;
    winrt::com_ptr<IMFMediaType>              m_outputMediaType;
    winrt::com_ptr<IMFVideoSampleAllocatorEx> m_sampleAllocator;
    winrt::com_ptr<IMFSample>                 m_outputSample;
    winrt::com_ptr<SourceReaderCallback>      m_callback {nullptr};
    UINT32                                    m_width {0};
    UINT32                                    m_height {0};
    bool                                      m_init {false};
    std::mutex                                m_mutex {};
    const bool                                m_async {true};
};