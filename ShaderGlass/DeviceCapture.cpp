/*
ShaderGlass: shader effect overlay
Copyright (C) 2021-2025 mausimus (mausimus.net)
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0
*/

#include "pch.h"

#include "DeviceCapture.h"
#include "Helpers.h"

#define THROW(h)                                                                                                                                                                   \
    if(FAILED(h))                                                                                                                                                                  \
        throw std::runtime_error("Unable to initialize Media Foundation Capture");

static HRESULT hr;

constexpr unsigned STREAM_NO = 0;

const std::map<unsigned long, int> FormatPriorities {
    {842094158, 3}, // NV12
    {1196444237, 0}, // MJPG
    {844715353, 2}, // YUY2
    {875967048, 0}, // H264
    {20, 1}}
; // RGB

DeviceCapture::DeviceCapture() { }

void DeviceCapture::Init()
{
    if(!m_init)
    {
        THROW(MFStartup(MF_VERSION, MFSTARTUP_LITE));
        m_init = true;
    }
}

std::vector<CaptureDevice> DeviceCapture::GetCaptureDevices()
{
    std::vector<CaptureDevice> result;

    winrt::com_ptr<IMFAttributes> attributes;
    UINT32                        numDevices = 0;
    IMFActivate**                 devices    = NULL;
    BOOL                          selected   = FALSE;

    Init();

    THROW(MFCreateAttributes(attributes.put(), 1));
    THROW(attributes->SetGUID(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE, MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID));
    THROW(MFEnumDeviceSources(attributes.get(), &devices, &numDevices));
    if(numDevices == 0)
        return result;

    try
    {
        for(unsigned deviceNo = 0; deviceNo < numDevices && deviceNo < MAX_CAPTURE_DEVICES; deviceNo++)
        {
            winrt::com_ptr<IMFMediaSource>            mediaSource;
            winrt::com_ptr<IMFPresentationDescriptor> presentationDescriptor;
            winrt::com_ptr<IMFStreamDescriptor>       streamDescriptor;
            winrt::com_ptr<IMFMediaTypeHandler>       mediaTypeHandler;
            winrt::com_ptr<IMFMediaType>              mediaType;

            THROW(devices[deviceNo]->ActivateObject(__uuidof(IMFMediaSource), reinterpret_cast<void**>(mediaSource.put())));
            THROW(mediaSource->CreatePresentationDescriptor(presentationDescriptor.put()));
            THROW(presentationDescriptor->GetStreamDescriptorByIndex(STREAM_NO, &selected, streamDescriptor.put()));
            if(!selected)
                throw std::runtime_error("Stream not selected");

            wchar_t deviceName[MAX_DEVICE_NAME];
            UINT32  deviceNameLen = MAX_DEVICE_NAME;
            devices[deviceNo]->GetString(MF_DEVSOURCE_ATTRIBUTE_FRIENDLY_NAME, deviceName, MAX_DEVICE_NAME, &deviceNameLen);

            CaptureDevice cdi {.no = deviceNo, .name = std::wstring(deviceName)};

            DWORD mediaTypeCount;
            THROW(streamDescriptor->GetMediaTypeHandler(mediaTypeHandler.put()));
            THROW(mediaTypeHandler->GetMediaTypeCount(&mediaTypeCount));

            std::map<unsigned, CaptureFormat> bestFormatByBucket;

            for(unsigned formatNo = 0; formatNo < mediaTypeCount; formatNo++)
            {
                THROW(mediaTypeHandler->GetMediaTypeByIndex(formatNo, mediaType.put()));

                GUID format;
                THROW(mediaType->GetGUID(MF_MT_SUBTYPE, &format));
                char formatCode[5];
                formatCode[0] = (format.Data1) & 127;
                formatCode[1] = (format.Data1 >> 8) & 127;
                formatCode[2] = (format.Data1 >> 16) & 127;
                formatCode[3] = (format.Data1 >> 24) & 127;
                formatCode[4] = 0;

                UINT32 w, h;
                THROW(MFGetAttributeSize(mediaType.get(), MF_MT_FRAME_SIZE, &w, &h));

                UINT32 num, denum;
                THROW(MFGetAttributeRatio(mediaType.get(), MF_MT_FRAME_RATE, &num, &denum));
                float fps = denum != 0 ? num / (float)denum : 0.0f;

                if(fps > 75)
                    continue;

                wchar_t formatName[MAX_DEVICE_NAME];
                //_snwprintf_s(formatName, MAX_DEVICE_NAME, L"%dx%d %.2f fps (%S)\n", w, h, fps, formatCode);
                _snwprintf_s(formatName, MAX_DEVICE_NAME, L"%dx%d @ %.2f fps", w, h, fps);

                char formatId[MAX_DEVICE_NAME];
                snprintf(formatId, MAX_DEVICE_NAME, "%u:%u:%lu:%u:%u", w, h, format.Data1, num, denum);

                int  priority      = 1;
                auto knownPriority = FormatPriorities.find(format.Data1);
                if(knownPriority != FormatPriorities.end())
                    priority = knownPriority->second;

                auto newFormat = CaptureFormat {.no = formatNo, .name = std::wstring(formatName), .id = std::string(formatId), .wh = w * h, .fps = fps, .priority = priority};

                unsigned bucket         = (w << 16) + h;
                auto     existingBucket = bestFormatByBucket.find(bucket);
                if(existingBucket != bestFormatByBucket.end())
                {
                    // check if this better
                    auto replace        = false;
                    auto existingFormat = existingBucket->second;
                    if(fps > existingFormat.fps)
                        replace = true;
                    else if(fps == existingFormat.fps && priority > existingFormat.priority)
                        replace = true;

                    if(replace)
                    {
                        bestFormatByBucket[bucket] = newFormat;
                    }
                }
                else
                {
                    bestFormatByBucket[bucket] = newFormat;
                }
            }

            for(auto& b : bestFormatByBucket)
            {
                cdi.formats.emplace_back(b.second);
            }

            std::sort(cdi.formats.begin(), cdi.formats.end(), [](const CaptureFormat& a, const CaptureFormat& b) { return a.wh > b.wh; });

            if(cdi.formats.size() > MAX_CAPTURE_FORMATS)
            {
                cdi.formats.resize(MAX_CAPTURE_FORMATS);
            }

            result.emplace_back(cdi);
        }
    }
    catch(std::exception&)
    {
        for(UINT32 i = 0; i < numDevices; i++)
            devices[i]->Release();
        CoTaskMemFree(devices);
        throw;
    }

    return result;
}

static DWORD WINAPI DeviceCaptureThreadFuncProxy(LPVOID lpParam)
{
    ((DeviceCapture*)lpParam)->ThreadFunc();
    return 0;
}

void DeviceCapture::ThreadFunc()
{
    while(m_active)
    {
        try
        {
            WaitForNextFrame();
        }
        catch(...)
        { }
    }
}

bool DeviceCapture::Poll()
{
    std::unique_lock lock(m_mutex);

    try
    {
        if(m_inputSample)
        {
            Process(m_inputSample.get());
            m_inputSample = nullptr;
            return true;
        }
    }
    catch(...)
    { }
    return false;
}

void DeviceCapture::Start(winrt::com_ptr<ID3D11Device> d3dDevice, int deviceNo, int formatNo)
{
    Init();

    CreateMediaSource(deviceNo, STREAM_NO, formatNo);
    CreateSourceReader();
    SetMediaType();
    CreateSampleAllocator(d3dDevice);
    CreateOutputTexture();

    m_active = true;
    m_thread = CreateThread(NULL, 0, DeviceCaptureThreadFuncProxy, this, 0, NULL);
}

void DeviceCapture::CreateOutputTexture()
{
    winrt::com_ptr<IMFMediaBuffer> mediaBuffer;
    winrt::com_ptr<IMFDXGIBuffer>  dxgiBuffer;

    THROW(m_outputSample->GetBufferByIndex(0, mediaBuffer.put()));
    THROW(mediaBuffer->QueryInterface(IID_PPV_ARGS(dxgiBuffer.put())));
    THROW(dxgiBuffer->GetResource(IID_PPV_ARGS(m_outputTexture.put())));
}

void DeviceCapture::CreateSourceReader()
{
    winrt::com_ptr<IMFAttributes> attributes;

    THROW(MFCreateAttributes(attributes.put(), 3));
    THROW(attributes->SetUINT32(MF_SOURCE_READER_ENABLE_ADVANCED_VIDEO_PROCESSING, 1));
    THROW(attributes->SetUINT32(MF_READWRITE_ENABLE_HARDWARE_TRANSFORMS, 1));
    THROW(attributes->SetUINT32(MF_LOW_LATENCY, 1));
    THROW(MFCreateSourceReaderFromMediaSource(m_mediaSource.get(), attributes.get(), m_sourceReader.put()));
}

void DeviceCapture::CreateMediaSource(unsigned deviceNo, unsigned streamNo, unsigned mediaNo)
{
    winrt::com_ptr<IMFAttributes>             attributes;
    winrt::com_ptr<IMFPresentationDescriptor> presentationDescriptor;
    winrt::com_ptr<IMFStreamDescriptor>       streamDescriptor;
    winrt::com_ptr<IMFMediaTypeHandler>       mediaTypeHandler;
    winrt::com_ptr<IMFMediaType>              mediaType;
    IMFActivate**                             devices    = NULL;
    UINT32                                    numDevices = 0;
    BOOL                                      selected   = FALSE;

    THROW(MFCreateAttributes(attributes.put(), 1));
    THROW(attributes->SetGUID(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE, MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID));
    THROW(MFEnumDeviceSources(attributes.get(), &devices, &numDevices));
    if(numDevices == 0 || numDevices <= deviceNo)
        throw std::runtime_error("Device not found");

    try
    {
        THROW(devices[deviceNo]->ActivateObject(__uuidof(IMFMediaSource), reinterpret_cast<void**>(m_mediaSource.put())));
        THROW(m_mediaSource->CreatePresentationDescriptor(presentationDescriptor.put()));
        THROW(presentationDescriptor->GetStreamDescriptorByIndex(streamNo, &selected, streamDescriptor.put()));
        if(!selected)
            throw std::runtime_error("Stream not selected");

        THROW(streamDescriptor->GetMediaTypeHandler(mediaTypeHandler.put()));

        DWORD mediaTypeCount;
        THROW(mediaTypeHandler->GetMediaTypeCount(&mediaTypeCount));
        if(mediaTypeCount == 0 || mediaTypeCount <= mediaNo)
            throw std::runtime_error("Device format not found");
        THROW(mediaTypeHandler->GetMediaTypeByIndex(mediaNo, mediaType.put()));
        THROW(mediaTypeHandler->SetCurrentMediaType(mediaType.get()));
    }
    catch(std::exception&)
    {
        for(UINT32 i = 0; i < numDevices; i++)
            devices[i]->Release();
        CoTaskMemFree(devices);
        throw;
    }
}

void DeviceCapture::SetMediaType()
{
    winrt::com_ptr<IMFMediaType> sourceMediaType;

    THROW(m_sourceReader->GetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, sourceMediaType.put()));
    THROW(MFGetAttributeSize(sourceMediaType.get(), MF_MT_FRAME_SIZE, &m_width, &m_height));
    THROW(MFCreateMediaType(m_outputMediaType.put()));
    THROW(m_outputMediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video));
    THROW(m_outputMediaType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB32));
    THROW(m_outputMediaType->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive));
    THROW(m_outputMediaType->SetUINT32(MF_MT_ALL_SAMPLES_INDEPENDENT, TRUE));
    THROW(MFSetAttributeRatio(m_outputMediaType.get(), MF_MT_PIXEL_ASPECT_RATIO, 1, 1));
    THROW(CopyAttribute(sourceMediaType.get(), m_outputMediaType.get(), MF_MT_FRAME_SIZE));
    THROW(CopyAttribute(sourceMediaType.get(), m_outputMediaType.get(), MF_MT_FRAME_RATE));
    THROW(m_sourceReader->SetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, NULL, m_outputMediaType.get()));
}

void DeviceCapture::CreateSampleAllocator(winrt::com_ptr<ID3D11Device> d3dDevice)
{
    winrt::com_ptr<IMFDXGIDeviceManager> dxgiDeviceManager;
    winrt::com_ptr<IMFMediaTypeHandler>  mediaTypeHandler;
    winrt::com_ptr<IMFAttributes>        attributes;
    UINT                                 resetToken;

    THROW(MFCreateDXGIDeviceManager(&resetToken, dxgiDeviceManager.put()));
    THROW(dxgiDeviceManager->ResetDevice(d3dDevice.get(), resetToken));
    THROW(MFCreateVideoSampleAllocatorEx(IID_PPV_ARGS(m_sampleAllocator.put())));
    THROW(m_sampleAllocator->SetDirectXManager(dxgiDeviceManager.get()));
    THROW(MFCreateAttributes(attributes.put(), 2));
    THROW(attributes->SetUINT32(MF_SA_D3D11_USAGE, 0 /*D3D11_USAGE_DEFAULT*/));
    THROW(attributes->SetUINT32(MF_SA_D3D11_BINDFLAGS, 8 /*D3D11_BIND_SHADER_RESOURCE*/));
    THROW(m_sampleAllocator->InitializeSampleAllocatorEx(1, 1, attributes.get(), m_outputMediaType.get()));
    THROW(m_sampleAllocator->AllocateSample(m_outputSample.put()));
}

void DeviceCapture::Process(IMFSample* inputSample)
{
    if(!m_active)
        return;

    winrt::com_ptr<IMFMediaBuffer> srcBuffer;
    winrt::com_ptr<IMFMediaBuffer> dstBuffer;
    winrt::com_ptr<IMF2DBuffer>    dstBuffer2D;
    BYTE*                          bufferData = NULL;
    DWORD                          bufferLen  = 0;

    THROW(inputSample->ConvertToContiguousBuffer(srcBuffer.put()));
    THROW(srcBuffer->Lock(&bufferData, NULL, &bufferLen));
    THROW(m_outputSample->GetBufferByIndex(0, dstBuffer.put()));
    THROW(dstBuffer->QueryInterface(IID_PPV_ARGS(dstBuffer2D.put())));
    THROW(dstBuffer2D->ContiguousCopyFrom(bufferData, bufferLen));
    THROW(srcBuffer->Unlock());
}

bool DeviceCapture::WaitForNextFrame()
{
    if(!m_active)
        return false;

    winrt::com_ptr<IMFSample> inputSample;
    DWORD                     streamIndex;
    DWORD                     streamFlags;
    LONGLONG                  streamTime;

    THROW(m_sourceReader->ReadSample((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, &streamIndex, &streamFlags, &streamTime, inputSample.put()));
    if(!inputSample || !m_active)
        return false;

    m_inputSample = inputSample;

    return true;
}

void DeviceCapture::Stop()
{
    std::unique_lock lock(m_mutex);

    if(m_active)
    {
        m_active = false;

        m_inputSample     = nullptr;
        m_outputSample    = nullptr;
        m_sampleAllocator = nullptr;
        m_outputMediaType = nullptr;
        m_sourceReader    = nullptr;
        m_mediaSource     = nullptr;
    }
}

HRESULT DeviceCapture::CopyAttribute(IMFAttributes* pFrom, IMFAttributes* pTo, REFGUID guidKey)
{
    PROPVARIANT val;
    pFrom->GetItem(guidKey, &val);
    if(SUCCEEDED(hr))
    {
        hr = pTo->SetItem(guidKey, val);
        PropVariantClear(&val);
    }
    else if(hr == MF_E_ATTRIBUTENOTFOUND)
    {
        hr = S_OK;
    }
    return hr;
}
