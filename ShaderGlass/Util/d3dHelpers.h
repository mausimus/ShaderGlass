#pragma once

namespace util::uwp
{
inline std::vector<winrt::com_ptr<IDXGIAdapter>> EnumerateAdapters(void)
{
    winrt::com_ptr<IDXGIAdapter>              pAdapter;
    std::vector<winrt::com_ptr<IDXGIAdapter>> vAdapters;
    IDXGIFactory1*                            pFactory = NULL;

    if(FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&pFactory)))
    {
        return vAdapters;
    }
    for(UINT i = 0; pFactory->EnumAdapters(i, pAdapter.put()) != DXGI_ERROR_NOT_FOUND; ++i)
    {
        vAdapters.push_back(pAdapter);
    }
    if(pFactory)
    {
        pFactory->Release();
    }
    return vAdapters;
}

inline auto CreateD3DDevice(IDXGIAdapter* adapter, D3D_DRIVER_TYPE const type, winrt::com_ptr<ID3D11Device>& device)
{
    WINRT_ASSERT(!device);

    UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_VIDEO_SUPPORT;

#ifdef _DEBUG
    flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    return D3D11CreateDevice(adapter, type, nullptr, flags, nullptr, 0, D3D11_SDK_VERSION, device.put(), nullptr, nullptr);
}

inline auto CreateD3DDevice(IDXGIAdapter* adapter)
{
    winrt::com_ptr<ID3D11Device> device;
    HRESULT                      hr = CreateD3DDevice(adapter, adapter ? D3D_DRIVER_TYPE_UNKNOWN : D3D_DRIVER_TYPE_HARDWARE, device);
    if(DXGI_ERROR_UNSUPPORTED == hr)
    {
        hr = CreateD3DDevice(nullptr, D3D_DRIVER_TYPE_WARP, device);
    }

    winrt::check_hresult(hr);
    return device;
}
} // namespace util::uwp