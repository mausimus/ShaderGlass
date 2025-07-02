#pragma once

#include <mfapi.h>
#include <mfidl.h>
#include <Mfreadwrite.h>
#include <mferror.h>

#include "Options.h"

class DeviceCapture
{
public:
    DeviceCapture();

    std::vector<CaptureDevice> GetCaptureDevices();
    void                       Start(winrt::com_ptr<ID3D11Device> d3dDevice, int deviceNo, int formatNo);
    void                       Stop();
    bool                       Poll();

    winrt::com_ptr<ID3D11Texture2D> m_outputTexture;
    bool                            m_active;

private:
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
    UINT32                                    m_width;
    UINT32                                    m_height;
    bool                                      m_init;
    std::mutex                                m_mutex {};
};