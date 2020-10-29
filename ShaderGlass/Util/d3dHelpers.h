#pragma once

namespace util::uwp
{
    inline std::vector <IDXGIAdapter*> EnumerateAdapters(void)
    {
        IDXGIAdapter* pAdapter;
        std::vector <IDXGIAdapter*> vAdapters;
        IDXGIFactory1* pFactory = NULL;

        if (FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&pFactory)))
        {
            return vAdapters;
        }
        for (UINT i = 0;
            pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND;
            ++i)
        {
            DXGI_ADAPTER_DESC desc;
            pAdapter->GetDesc(&desc);
            vAdapters.push_back(pAdapter);
        }
        if (pFactory)
        {
            pFactory->Release();
        }
        return vAdapters;
    }

    inline auto CreateD3DDevice(D3D_DRIVER_TYPE const type, winrt::com_ptr<ID3D11Device>& device)
    {
        WINRT_ASSERT(!device);

        UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

        #ifdef _DEBUG
            	flags |= D3D11_CREATE_DEVICE_DEBUG;
        #endif

        return D3D11CreateDevice(nullptr, type, nullptr, flags, nullptr, 0, D3D11_SDK_VERSION, device.put(),
            nullptr, nullptr);
    }

    inline auto CreateD3DDevice()
    {
        winrt::com_ptr<ID3D11Device> device;
        HRESULT hr = CreateD3DDevice(D3D_DRIVER_TYPE_HARDWARE, device);
        if (DXGI_ERROR_UNSUPPORTED == hr)
        {
            hr = CreateD3DDevice(D3D_DRIVER_TYPE_WARP, device);
        }

        winrt::check_hresult(hr);
        return device;
    }
}