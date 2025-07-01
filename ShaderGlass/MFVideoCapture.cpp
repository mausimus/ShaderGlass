/*
ShaderGlass: shader effect overlay
Copyright (C) 2021-2025 mausimus (mausimus.net)
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0
*/

#include "pch.h"

#include "MFVideoCapture.h"

#pragma comment(lib, "mf.lib")
#pragma comment(lib, "mfuuid")

#define THROW(h)                                                                                                                                                                   \
    if(FAILED(h))                                                                                                                                                                  \
        throw std::runtime_error("Unable to initialize MF Capture");

static HRESULT hr;

constexpr unsigned STREAM_NO = 0;

void MFVideoCapture::Init()
{
    THROW(MFStartup(MF_VERSION, MFSTARTUP_LITE));
}

std::vector<CaptureDeviceInfo> MFVideoCapture::GetDevicesAndFormats()
{
    std::vector<CaptureDeviceInfo> result;

    winrt::com_ptr<IMFAttributes> attributes;
    UINT32                        numDevices = 0;
    IMFActivate**                 devices    = NULL;
    BOOL                          selected   = FALSE;

    THROW(MFCreateAttributes(attributes.put(), 1));
    THROW(attributes->SetGUID(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE, MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID));
    THROW(MFEnumDeviceSources(attributes.get(), &devices, &numDevices));
    if(numDevices == 0)
        return result;

    try
    {
        for(unsigned deviceNo = 0; deviceNo < numDevices; deviceNo++)
        {
            winrt::com_ptr<IMFMediaSource>            mediaSource;
            winrt::com_ptr<IMFPresentationDescriptor> presentationDescriptor;
            winrt::com_ptr<IMFStreamDescriptor>       streamDescriptor;
            winrt::com_ptr<IMFMediaTypeHandler>       mediaTypeHandler;
            winrt::com_ptr<IMFMediaType>              mediaType;

            CaptureDeviceInfo cdi;

            THROW(devices[deviceNo]->ActivateObject(__uuidof(IMFMediaSource), reinterpret_cast<void**>(mediaSource.put())));
            THROW(mediaSource->CreatePresentationDescriptor(presentationDescriptor.put()));
            THROW(presentationDescriptor->GetStreamDescriptorByIndex(STREAM_NO, &selected, streamDescriptor.put()));
            if(!selected)
                throw std::runtime_error("Stream not selected");

            wchar_t deviceName[1024];
            UINT32  deviceNameLen = 1024;
            devices[deviceNo]->GetString(MF_DEVSOURCE_ATTRIBUTE_FRIENDLY_NAME, deviceName, 1024, &deviceNameLen);
            cdi.m_name = std::wstring(deviceName);

            DWORD mediaTypeCount;
            THROW(streamDescriptor->GetMediaTypeHandler(mediaTypeHandler.put()));
            THROW(mediaTypeHandler->GetMediaTypeCount(&mediaTypeCount));

            for(unsigned i = 0; i < mediaTypeCount; i++)
            {
                THROW(mediaTypeHandler->GetMediaTypeByIndex(i, mediaType.put()));

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

                wchar_t formatName[50];
                _snwprintf_s(formatName, 50, L"%dx%d %.2f fps (%S)\n", w, h, fps, formatCode);

                cdi.m_formats.push_back(CaptureFormatInfo {.m_no = i, .m_name = std::wstring(formatName)});
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

void MFVideoCapture::Start(winrt::com_ptr<ID3D11Device> d3dDevice, int deviceNo, int formatNo)
{
    CreateMediaSource(deviceNo, STREAM_NO, formatNo);
    CreateSourceReader();
    SetMediaType();
    CreateSampleAllocator(d3dDevice);
    CreateOutputTexture();
}

void MFVideoCapture::CreateOutputTexture()
{
    winrt::com_ptr<IMFMediaBuffer> mediaBuffer;
    winrt::com_ptr<IMFDXGIBuffer>  dxgiBuffer;

    THROW(m_outputSample->GetBufferByIndex(0, mediaBuffer.put()));
    THROW(mediaBuffer->QueryInterface(IID_PPV_ARGS(dxgiBuffer.put())));
    THROW(dxgiBuffer->GetResource(IID_PPV_ARGS(m_outputTexture.put())));
}

void MFVideoCapture::CreateSourceReader()
{
    winrt::com_ptr<IMFAttributes> attributes;

    THROW(MFCreateAttributes(attributes.put(), 1));
    THROW(attributes->SetUINT32(MF_SOURCE_READER_ENABLE_VIDEO_PROCESSING, 1));
    THROW(MFCreateSourceReaderFromMediaSource(m_mediaSource.get(), attributes.get(), m_sourceReader.put()));
}

void MFVideoCapture::CreateMediaSource(unsigned deviceNo, unsigned streamNo, unsigned mediaNo)
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

void MFVideoCapture::SetMediaType()
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

void MFVideoCapture::CreateSampleAllocator(winrt::com_ptr<ID3D11Device> d3dDevice)
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

bool MFVideoCapture::Poll()
{
    winrt::com_ptr<IMFSample>      inputSample;
    winrt::com_ptr<IMFMediaBuffer> srcBuffer;
    winrt::com_ptr<IMFMediaBuffer> dstBuffer;
    winrt::com_ptr<IMF2DBuffer>    dstBuffer2D;
    DWORD                          streamIndex;
    DWORD                          streamFlags;
    LONGLONG                       streamTime;
    BYTE*                          bufferData = NULL;
    DWORD                          bufferLen  = 0;

    THROW(m_sourceReader->ReadSample((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, &streamIndex, &streamFlags, &streamTime, inputSample.put()));
    if(!inputSample)
        return false;

    THROW(inputSample->ConvertToContiguousBuffer(srcBuffer.put()));
    THROW(srcBuffer->Lock(&bufferData, NULL, &bufferLen));
    THROW(m_outputSample->GetBufferByIndex(0, dstBuffer.put()));
    THROW(dstBuffer->QueryInterface(IID_PPV_ARGS(dstBuffer2D.put())));
    THROW(dstBuffer2D->ContiguousCopyFrom(bufferData, bufferLen));
    THROW(srcBuffer->Unlock());

    return true;
}

HRESULT MFVideoCapture::CopyAttribute(IMFAttributes* pFrom, IMFAttributes* pTo, REFGUID guidKey)
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