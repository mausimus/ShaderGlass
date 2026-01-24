#pragma once

#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <mferror.h>

#include "Options.h"

#define MAX_CAPTURE_DEVICES 16U
#define MAX_CAPTURE_FORMATS 256U

#ifndef SG_WINE

class DeviceCapture
{
public:
    DeviceCapture();

    std::vector<CaptureDevice>      GetCaptureDevices();
    void                            Start(winrt::com_ptr<ID3D11Device> d3dDevice, LPWSTR symlink, int formatNo);
    void                            Stop();
    bool                            WaitForNextFrame();
    bool                            Poll();
    void                            ThreadFunc();
    winrt::com_ptr<ID3D11Texture2D> m_outputTexture;
    bool                            m_active {false};

private:
    void Init();
    void CreateMediaSource(LPWSTR symlink, unsigned streamNo, unsigned mediaNo);
    void CreateSourceReader();
    void SetMediaType();
    void CreateSampleAllocator(winrt::com_ptr<ID3D11Device>);
    void CreateOutputTexture();
    void Process(IMFSample* inputSample);

    HRESULT CopyAttribute(IMFAttributes*, IMFAttributes*, REFGUID);

    winrt::com_ptr<IMFMediaSource>            m_mediaSource;
    winrt::com_ptr<IMFSourceReader>           m_sourceReader;
    winrt::com_ptr<IMFMediaType>              m_outputMediaType;
    winrt::com_ptr<IMFVideoSampleAllocatorEx> m_sampleAllocator;
    winrt::com_ptr<IMFSample>                 m_outputSample;
    winrt::com_ptr<IMFSample>                 m_inputSample;
    UINT32                                    m_width {0};
    UINT32                                    m_height {0};
    HANDLE                                    m_thread {0};
    bool                                      m_init {false};
    std::mutex                                m_mutex {};
};

#else

class DeviceCapture
{
public:
    DeviceCapture(){}

    std::vector<CaptureDevice>      GetCaptureDevices() { return std::vector<CaptureDevice>(); }
    void                            Start(winrt::com_ptr<ID3D11Device> d3dDevice, LPWSTR symlink, int formatNo) {}
    void                            Stop() {}
    bool                            WaitForNextFrame() {return false;}
    bool                            Poll() {return false;}
    void                            ThreadFunc() {}
    winrt::com_ptr<ID3D11Texture2D> m_outputTexture;
    bool                            m_active {false};
};

#endif
