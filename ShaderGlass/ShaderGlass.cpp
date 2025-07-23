/*
ShaderGlass: shader effect overlay
Copyright (C) 2021-2025 mausimus (mausimus.net)
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0
*/

#include "pch.h"
#include "ShaderGlass.h"
#include "ShaderList.h"
#include "CursorEmulator.h"
#include "resource.h"

static HRESULT     hr;
static const float background_colour[4] = {0, 0, 0, 1.0f};

ShaderGlass::ShaderGlass(CursorEmulator& cursorEmulator) :
    m_lastSize {}, m_lastPos {}, m_lastCaptureWindowPos {}, m_lastCaptureWindowSize {}, m_passthroughDef(), m_shaderPreset(new Preset(m_passthroughDef)),
    m_preprocessShader(m_preprocessShaderDef), m_preprocessPreset(m_preprocessPresetDef), m_preprocessPass(m_preprocessShader, m_preprocessPreset, true),
    m_cursorEmulator(cursorEmulator)
{ }

ShaderGlass::~ShaderGlass()
{
    std::unique_lock lock(m_mutex);

    DestroyShaders();
    DestroyPasses();
    DestroyTargets();

    m_context->Flush();
}

void ShaderGlass::Initialize(HWND                                outputWindow,
                             HWND                                captureWindow,
                             HMONITOR                            captureMonitor,
                             bool                                clone,
                             bool                                image,
                             bool                                flipMode,
                             bool                                allowTearing,
                             bool                                useHDR,
                             winrt::com_ptr<ID3D11Device>        device,
                             winrt::com_ptr<ID3D11DeviceContext> context)
{
    m_outputWindow  = outputWindow;
    m_captureWindow = captureWindow;
    m_clone         = clone;
    m_image         = image;
    m_flipMode      = flipMode;
    m_allowTearing  = allowTearing;
    m_useHDR        = useHDR;
    m_device        = device;
    m_context       = context;

    if(captureMonitor && !clone)
    {
        MONITORINFO monitorInfo;
        monitorInfo.cbSize = sizeof(MONITORINFO);
        GetMonitorInfo(captureMonitor, &monitorInfo);
        m_monitorOffset.x = monitorInfo.rcMonitor.left;
        m_monitorOffset.y = monitorInfo.rcMonitor.top;
    }
    else if(!captureWindow && !captureMonitor && !clone)
    {
        // All Desktops glass
        m_monitorOffset.x = GetSystemMetrics(SM_XVIRTUALSCREEN);
        m_monitorOffset.y = GetSystemMetrics(SM_YVIRTUALSCREEN);
    }
    else
    {
        m_monitorOffset.x = 0;
        m_monitorOffset.y = 0;
    }

    // remember initial size
    m_lastPos.x = 0;
    m_lastPos.y = 0;
    RECT clientRect;
    GetClientRect(m_outputWindow, &clientRect);
    m_lastSize.x = clientRect.right;
    m_lastSize.y = clientRect.bottom;

    m_prevTicks          = GetTickCount64();
    m_startTicks         = GetTickCount64();
    m_prevLogicalFrameNo = 0;

    // create swapchain
    {
        winrt::com_ptr<IDXGIFactory2> dxgiFactory;
        {
            winrt::com_ptr<IDXGIDevice1> dxgiDevice;
            hr = m_device->QueryInterface(__uuidof(IDXGIDevice1), (void**)dxgiDevice.put());
            assert(SUCCEEDED(hr));

            winrt::com_ptr<IDXGIAdapter> dxgiAdapter;
            hr = dxgiDevice->GetAdapter(dxgiAdapter.put());
            assert(SUCCEEDED(hr));

            hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), (void**)dxgiFactory.put());
            assert(SUCCEEDED(hr));
        }

        DXGI_SWAP_CHAIN_DESC1 d3d11SwapChainDesc = {};
        d3d11SwapChainDesc.Width                 = 0;
        d3d11SwapChainDesc.Height                = 0;
        d3d11SwapChainDesc.Format                = m_useHDR ? DXGI_FORMAT_R16G16B16A16_FLOAT : DXGI_FORMAT_B8G8R8A8_UNORM;
        d3d11SwapChainDesc.SampleDesc.Count      = 1;
        d3d11SwapChainDesc.SampleDesc.Quality    = 0;
        d3d11SwapChainDesc.BufferUsage           = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        d3d11SwapChainDesc.BufferCount           = 3;
        // flip mode has a weird bug on Win 10 where the first frame doesn't align with the window client area, until window is moved :(
        if(m_flipMode)
        {
            d3d11SwapChainDesc.Scaling    = DXGI_SCALING_NONE;
            d3d11SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        }
        else
        {
            d3d11SwapChainDesc.Scaling    = DXGI_SCALING_STRETCH;
            d3d11SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        }
        d3d11SwapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
        if(m_flipMode && m_allowTearing)
        {
            d3d11SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
        }
        else
        {
            d3d11SwapChainDesc.Flags = 0;
        }

        hr = dxgiFactory->CreateSwapChainForHwnd(m_device.get(), m_outputWindow, &d3d11SwapChainDesc, 0, 0, m_swapChain.put());
        assert(SUCCEEDED(hr));
    }

    hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)m_displayTexture.put());
    assert(SUCCEEDED(hr));
    if(!m_displayTexture)
        throw std::exception("Unable to create framebuffer");

    m_device->CreateRenderTargetView(m_displayTexture.get(), 0, m_displayRenderTarget.put());

    D3D11_RASTERIZER_DESC desc = {};
    desc.CullMode              = D3D11_CULL_NONE;
    desc.FillMode              = D3D11_FILL_SOLID;
    desc.DepthClipEnable       = FALSE;
    desc.MultisampleEnable     = FALSE;
    hr                         = m_device->CreateRasterizerState(&desc, m_rasterizerState.put());
    assert(SUCCEEDED(hr));

    if(m_useHDR)
    {
        hr = m_swapChain->QueryInterface(__uuidof(IDXGISwapChain3), reinterpret_cast<void**>(m_swapChain3.put()));
        assert(SUCCEEDED(hr));

        SetSwapchainColorSpace();
    }

    m_context->RSSetState(m_rasterizerState.get());

    m_preprocessShader.Create(m_device);
    m_preprocessPass.Initialize(m_device, m_context);
    RebuildShaders();

    m_running = true;
}

void ShaderGlass::RebuildShaders()
{
    m_shaderPreset->Create(m_device);
    m_shaderPasses.reserve(m_shaderPreset->m_shaders.size() + (m_vertical ? 1 : 0));
    for(auto& shader : m_shaderPreset->m_shaders)
    {
        m_shaderPasses.emplace_back(shader, *m_shaderPreset, m_device, m_context);
    }
    if(m_vertical)
    {
        m_shaderPasses.emplace_back(m_preprocessShader, m_preprocessPreset, m_device, m_context);
    }
    float vertical = m_vertical ? 1.0f : 0.0f;
    m_preprocessShader.SetParam("SGVertical", &vertical);

    m_presetTextures.clear();
    for(auto& texture : m_shaderPreset->m_textures)
    {
        m_presetTextures.insert(make_pair(texture.second.m_name, texture.second.m_textureView));
    }

    ResetParams();
}

void ShaderGlass::SetInputScale(float w, float h)
{
    m_inputScaleW   = w;
    m_inputScaleH   = h;
    m_inputRescaled = true;
}

void ShaderGlass::SetOutputScale(float w, float h)
{
    m_outputScaleW   = w;
    m_outputScaleH   = h;
    m_outputRescaled = true;
}

void ShaderGlass::SetOutputFlip(bool h, bool v)
{
    m_flipHorizontal = h;
    m_flipVertical   = v;
    m_outputRescaled = true;
}

void ShaderGlass::SetShaderPreset(PresetDef* p, const std::vector<std::tuple<int, std::string, double>>& params)
{
    m_newShaderPreset = std::make_unique<Preset>(*p);
    m_newParams       = params;
}

void ShaderGlass::SetFrameSkip(int s)
{
    m_frameSkip = s;
}

void ShaderGlass::SetLockedArea(RECT lockedArea)
{
    m_lockedArea.top    = lockedArea.top;
    m_lockedArea.bottom = lockedArea.bottom;
    m_lockedArea.left   = lockedArea.left;
    m_lockedArea.right  = lockedArea.right;
    m_lockedAreaUpdated = true;
}

void ShaderGlass::SetCroppedArea(RECT croppedArea)
{
    if(m_croppedArea.top != croppedArea.top || m_croppedArea.bottom != croppedArea.bottom || m_croppedArea.left != croppedArea.left || m_croppedArea.right != croppedArea.right)
    {
        m_croppedArea.top    = croppedArea.top;
        m_croppedArea.bottom = croppedArea.bottom;
        m_croppedArea.left   = croppedArea.left;
        m_croppedArea.right  = croppedArea.right;
        m_croppedAreaUpdated = true;
    }
}

void ShaderGlass::SetFreeScale(bool freeScale)
{
    m_freeScale      = freeScale;
    m_outputRescaled = true;
}

void ShaderGlass::SetVertical(bool vertical)
{
    if(m_vertical != vertical)
    {
        m_vertical        = vertical;
        m_verticalUpdated = true;
    }
}

void ShaderGlass::DestroyTargets()
{
    if(m_preprocessedRenderTarget != nullptr)
    {
        m_preprocessedRenderTarget = nullptr;
        m_originalView             = nullptr;
        m_preprocessedTexture      = nullptr;
    }
}

void ShaderGlass::UpdateParams()
{
    for(auto& s : m_shaderPreset->m_shaders)
        for(auto& p : s.Params())
        {
            if(p->size == 4 && p->name != "FrameCount")
                s.SetParam(p, &p->currentValue);
        }
}

float ShaderGlass::GetDefaultValue(ShaderParam* p)
{
    for(auto& o : m_shaderPreset->m_presetDef.Overrides)
    {
        if(o.name == p->name)
        {
            return o.value;
        }
    }
    return p->defaultValue;
}

void ShaderGlass::ResetParams()
{
    for(auto& s : m_shaderPreset->m_shaders)
        for(auto& p : s.Params())
        {
            if(p->size == 4 && p->name != "FrameCount")
            {
                // check for preset override
                auto hasOverride = false;
                for(auto& o : m_shaderPreset->m_presetDef.Overrides)
                {
                    if(o.name == p->name)
                    {
                        s.SetParam(p, &o.value);
                        hasOverride = true;
                        break;
                    }
                }
                if(!hasOverride)
                {
                    s.SetParam(p, &p->defaultValue);
                }
            }
        }
}

std::vector<std::tuple<int, ShaderParam*>> ShaderGlass::Params()
{
    std::vector<std::tuple<int, ShaderParam*>> params;
    int                                        i = 0;
    for(auto& s : m_shaderPreset->m_shaders)
    {
        for(auto& p : s.Params())
            if(p->size == 4 && p->name != "FrameCount")
                params.push_back(std::make_tuple(i, p));

        i++;
    }
    return params;
}

void ShaderGlass::SetSwapchainColorSpace()
{
    assert(m_swapChain3.get());

    if(m_swapChain3.get())
    {
        hr = m_swapChain3->SetColorSpace1(DXGI_COLOR_SPACE_RGB_FULL_G10_NONE_P709);
        assert(SUCCEEDED(hr));
    }
}

bool ShaderGlass::TryResizeSwapChain(const RECT& clientRect, bool force)
{
    if(force || (clientRect.right != m_lastSize.x) || (clientRect.bottom != m_lastSize.y))
    {
        // force recreate
        DestroyTargets();

        m_lastSize.x = clientRect.right;
        m_lastSize.y = clientRect.bottom;

        m_displayTexture      = nullptr;
        m_displayRenderTarget = nullptr;

        if(clientRect.right > 0 && clientRect.bottom > 0)
        {
            UINT flags = 0;
            if(m_flipMode && m_allowTearing)
                flags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
            hr = m_swapChain->ResizeBuffers(0, static_cast<UINT>(clientRect.right), static_cast<UINT>(clientRect.bottom), DXGI_FORMAT_UNKNOWN, flags);
            assert(SUCCEEDED(hr));

            hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)m_displayTexture.put());
            assert(SUCCEEDED(hr));

            hr = m_device->CreateRenderTargetView(m_displayTexture.get(), NULL, m_displayRenderTarget.put());
            assert(SUCCEEDED(hr));

            if(m_useHDR)
            {
                SetSwapchainColorSpace();
            }
        }
        return true;
    }
    return false;
}

void ShaderGlass::DestroyShaders()
{
    m_shaderPasses.clear();
}

void ShaderGlass::DestroyPasses()
{
    for(auto& rs : m_passResources)
    {
        if(rs.first.starts_with("PassOutput") || rs.first.starts_with("PassFeedback"))
        {
            rs.second = nullptr;
        }
    }
    m_passTargets.clear();
    m_passTextures.clear();
    m_passResources.clear();
    m_requiresFeedback = false;
    m_requiresHistory  = 0;
}

void ShaderGlass::PresentFrame()
{
    DXGI_PRESENT_PARAMETERS presentParameters {};
    UINT                    presentFlags = 0;
    if(m_flipMode)
    {
        presentFlags |= DXGI_PRESENT_RESTART;
        if(m_allowTearing)
        {
            presentFlags |= DXGI_PRESENT_ALLOW_TEARING;
        }
    }
    m_swapChain->Present1(0, presentFlags, &presentParameters);
    PostMessage(m_outputWindow, WM_PAINT, 0, 0); // necessary for click-through
}

void ShaderGlass::Process(winrt::com_ptr<ID3D11Texture2D> texture, ULONGLONG frameTicks, int inputFrameNo)
{
    auto nowTicks            = GetTickCount64();
    auto timeSinceLastRender = nowTicks - m_prevRenderTicks;
    auto logicalFrameNo      = (int)roundf((nowTicks - m_startTicks) / 16.6666666f); // fix shaders at 60 fps

    // same input
    if(inputFrameNo == m_prevInputFrameNo)
    {
        if(logicalFrameNo == m_prevLogicalFrameNo)
            return;

        auto timeSinceLastInput = nowTicks - frameTicks;
        if(timeSinceLastInput < 20) // 3.3 ms delay allowance for frame timing
            return;
    }

    if(m_frameSkip > 0)
    {
        if(logicalFrameNo == m_prevLogicalFrameNo) // already rendered
            return;

        if((logicalFrameNo % (m_frameSkip + 1) != 0)) // don't need this frame
            return;
    }

    m_frameCounter++;
    m_prevFrameTicks     = frameTicks;
    m_prevInputFrameNo   = inputFrameNo;
    m_prevLogicalFrameNo = logicalFrameNo;

    if(!m_running || !texture)
    {
        // skip frame
        PresentFrame();
        return;
    }

    std::unique_lock lock(m_mutex, std::try_to_lock);
    if(!lock.owns_lock())
    {
        // still rendering, drop frame
        return;
    }

    POINT topLeft;
    topLeft.x = 0;
    topLeft.y = 0;
    ClientToScreen(m_outputWindow, &topLeft);

    if(!m_captureWindow && !m_clone)
    {
        // desktop glass
        topLeft.x -= m_monitorOffset.x;
        topLeft.y -= m_monitorOffset.y;
    }

    RECT clientRect;
    GetClientRect(m_outputWindow, &clientRect);

    RECT outputRect;
    GetWindowRect(m_outputWindow, &outputRect);

    D3D11_TEXTURE2D_DESC capturedTextureDesc = {};
    texture->GetDesc(&capturedTextureDesc);

    auto inputResized    = m_captureWindow && m_croppedAreaUpdated;
    m_croppedAreaUpdated = false;

    // properties of the window being captured
    RECT  captureRect;
    POINT captureTopLeft;
    RECT  captureClient;
    captureTopLeft.x = 0;
    captureTopLeft.y = 0;
    bool outputMoved = false;
    if(m_captureWindow)
    {
        ClientToScreen(m_captureWindow, &captureTopLeft);
        GetClientRect(m_captureWindow, &captureClient);

        DwmGetWindowAttribute(m_captureWindow, DWMWA_EXTENDED_FRAME_BOUNDS, &captureRect, sizeof(RECT));
        captureTopLeft.x += m_croppedArea.left;
        captureTopLeft.y += m_croppedArea.top;
        captureClient.right -= (m_croppedArea.left + m_croppedArea.right);
        captureClient.bottom -= (m_croppedArea.top + m_croppedArea.bottom);
        if(captureClient.right <= 0)
            captureClient.right = 1;
        if(captureClient.bottom <= 0)
            captureClient.bottom = 1;

        outputMoved = (m_lastCaptureWindowPos.x != captureRect.left || m_lastCaptureWindowPos.y != captureRect.bottom);
        if(outputMoved)
        {
            m_lastCaptureWindowPos.x = captureRect.left;
            m_lastCaptureWindowPos.y = captureRect.bottom;
        }

        inputResized |= m_lastCaptureWindowSize.x != capturedTextureDesc.Width || m_lastCaptureWindowSize.y != capturedTextureDesc.Height;
        if(inputResized)
        {
            m_lastCaptureWindowSize.x = capturedTextureDesc.Width;
            m_lastCaptureWindowSize.y = capturedTextureDesc.Height;
        }
    }
    else if(m_image)
    {
        captureRect.left   = 0;
        captureRect.top    = 0;
        captureRect.right  = capturedTextureDesc.Width;
        captureRect.bottom = capturedTextureDesc.Height;
        captureClient      = captureRect;
    }

    RECT textureRect;
    textureRect.right  = capturedTextureDesc.Width;
    textureRect.bottom = capturedTextureDesc.Height;

    auto outputResized = false;
    outputResized      = TryResizeSwapChain(clientRect, m_outputRescaled);

    if(clientRect.right <= 0 || clientRect.bottom <= 0)
    {
        // skip
        PresentFrame();
        return;
    }

    auto clientWidth  = clientRect.right;
    auto clientHeight = clientRect.bottom;

    float boxX = 0, boxY = 0;
    if(m_captureWindow || m_image)
    {
        const auto captureW = (captureClient.right - captureClient.left);
        const auto captureH = (captureClient.bottom - captureClient.top);

        if(!m_freeScale)
        {
            clientWidth  = (LONG)roundf(captureW / m_outputScaleW);
            clientHeight = (LONG)roundf(captureH / m_outputScaleH);
        }

        // box if needed
        if(captureW != 0 && captureH != 0)
        {
            auto inputAspectRatio  = captureW / (float)captureH;
            auto outputAspectRatio = (clientWidth * m_outputScaleW) / (clientHeight * m_outputScaleH);
            if(outputAspectRatio > inputAspectRatio)
            {
                // output is wider
                auto newWidth = (LONG)roundf(clientHeight * (m_outputScaleH / m_outputScaleW) * inputAspectRatio);
                boxX          = (clientWidth - newWidth) / 2.0f;
                clientWidth   = newWidth;
            }
            else if(outputAspectRatio < inputAspectRatio)
            {
                // output is narrower
                auto newHeight = (LONG)roundf(clientWidth * (m_outputScaleW / m_outputScaleH) / inputAspectRatio);
                boxY           = (clientHeight - newHeight) / 2.0f;
                clientHeight   = newHeight;
            }

            // center (fullscreen?)
            if(!m_freeScale)
            {
                boxX += (clientRect.right - (captureW / m_outputScaleW)) / 2.0f;
                boxY += (clientRect.bottom - (captureH / m_outputScaleH)) / 2.0f;
            }

            if(boxX < 0)
                boxX = 0;
            if(boxY < 0)
                boxY = 0;
        }
    }
    m_boxX = static_cast<int>(boxX);
    m_boxY = static_cast<int>(boxY);

    // final window/viewport size
    UINT viewportWidth  = static_cast<UINT>(clientWidth);
    UINT viewportHeight = static_cast<UINT>(clientHeight);

    auto destWidth  = static_cast<long>(clientWidth * m_outputScaleW);
    auto destHeight = static_cast<long>(clientHeight * m_outputScaleH);

    if(destWidth <= (int)m_inputScaleW || destHeight <= (int)m_inputScaleH)
        return;

    bool inputRescaled = m_inputRescaled;
    m_inputRescaled    = false;
    m_outputRescaled   = false;

    // force recreate
    if(inputRescaled || inputResized || m_verticalUpdated)
    {
        if(m_preprocessedRenderTarget != nullptr)
        {
            DestroyTargets();
            if(m_displayRenderTarget)
            {
                m_context->ClearRenderTargetView(m_displayRenderTarget.get(), background_colour);
            }
        }
        else
        {
            // clear any blanks around captured window
            m_context->ClearRenderTargetView(m_displayRenderTarget.get(), background_colour);
        }
    }

    bool rebuildPasses = false;

    if(m_newShaderPreset || m_verticalUpdated)
    {
        m_startTicks = GetTickCount64(); // reset logical frame no

        DestroyShaders();
        if(m_newShaderPreset)
        {
            m_shaderPreset.swap(m_newShaderPreset);
            m_newShaderPreset.reset();
        }
        RebuildShaders();
        if(m_newParams.size())
        {
            const auto& shaderParams = Params();
            for(const auto& ip : m_newParams)
            {
                for(const auto& sp : shaderParams)
                {
                    if(get<0>(ip) == get<0>(sp) && get<1>(ip) == get<1>(sp)->name)
                    {
                        get<1>(sp)->currentValue = (float)get<2>(ip);
                        break;
                    }
                }
            }
            m_newParams.clear();
            UpdateParams();
        }
        PostMessage(m_outputWindow, WM_COMMAND, IDM_UPDATE_PARAMS, 0);
        inputRescaled     = true;
        outputResized     = true;
        rebuildPasses     = true;
        m_verticalUpdated = false;
    }

    // size of preprocessed input, which is 'original' for the shader chain
    UINT originalWidth  = static_cast<UINT>(destWidth / m_inputScaleW);
    UINT originalHeight = static_cast<UINT>(destHeight / m_inputScaleH);

    if(m_captureWindow || m_image)
    {
        const auto captureW = captureClient.right;
        const auto captureH = captureClient.bottom;
        originalWidth       = static_cast<UINT>(captureW / m_inputScaleW);
        originalHeight      = static_cast<UINT>(captureH / m_inputScaleH);
    }

    // create preprocessed output texture, scaled down size, inverted etc.
    if(m_preprocessedTexture == nullptr)
    {
        D3D11_TEXTURE2D_DESC desc2 = {};
        texture->GetDesc(&desc2);
        desc2.Usage          = D3D11_USAGE_DEFAULT;
        desc2.BindFlags      = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
        desc2.CPUAccessFlags = 0;
        desc2.MiscFlags      = 0;
        desc2.Width          = m_vertical ? originalHeight : originalWidth;
        desc2.Height         = m_vertical ? originalWidth : originalHeight;

        hr = m_device->CreateTexture2D(&desc2, nullptr, m_preprocessedTexture.put());
        assert(SUCCEEDED(hr));
        outputResized = true;
        rebuildPasses = true;

        hr = m_device->CreateShaderResourceView(m_preprocessedTexture.get(), nullptr, m_originalView.put());
        assert(SUCCEEDED(hr));
    }

    // create texture render target
    if(m_preprocessedRenderTarget == nullptr)
    {
        hr = m_device->CreateRenderTargetView(m_preprocessedTexture.get(), NULL, m_preprocessedRenderTarget.put());
        assert(SUCCEEDED(hr));
        rebuildPasses = true;
    }

    if(inputRescaled || outputResized || inputResized)
    {
        if(m_vertical)
        {
            std::swap(originalWidth, originalHeight);
            std::swap(viewportWidth, viewportHeight);
        }

        m_textureSizes.clear();
        m_textureSizes.insert(std::make_pair("Original", float4 {(float)originalWidth, (float)originalHeight, 1.0f / originalWidth, 1.0f / originalHeight}));
        m_textureSizes.insert(std::make_pair("FinalViewport", float4 {(float)viewportWidth, (float)viewportHeight, 1.0f / viewportWidth, 1.0f / viewportHeight}));

        // preprocess takes original texture full size
        std::vector<std::array<UINT, 4>> passSizes;
        m_preprocessPass.Resize(capturedTextureDesc.Width, capturedTextureDesc.Height, originalWidth, originalHeight, m_textureSizes, passSizes);

        UINT sourceWidth  = originalWidth;
        UINT sourceHeight = originalHeight;
        for(int p = 0; p < m_shaderPasses.size(); p++)
        {
            auto& shaderPass = m_shaderPasses[p];
            if(p == m_shaderPasses.size() - 1) // last shader scales source to viewport
            {
                if(m_vertical)
                {
                    std::swap(originalWidth, originalHeight);
                    std::swap(viewportWidth, viewportHeight);
                }
                passSizes.push_back({sourceWidth, sourceHeight, viewportWidth, viewportHeight});
            }
            else
            {
                UINT outputWidth  = sourceWidth;
                UINT outputHeight = sourceHeight;
                if(shaderPass.m_shader.m_scaleViewportX)
                    outputWidth = static_cast<UINT>(viewportWidth * shaderPass.m_shader.m_scaleX);
                else if(shaderPass.m_shader.m_scaleAbsoluteX)
                    outputWidth = static_cast<UINT>(shaderPass.m_shader.m_scaleX);
                else
                    outputWidth = static_cast<UINT>(sourceWidth * shaderPass.m_shader.m_scaleX);
                if(shaderPass.m_shader.m_scaleViewportY)
                    outputHeight = static_cast<UINT>(viewportHeight * shaderPass.m_shader.m_scaleY);
                else if(shaderPass.m_shader.m_scaleAbsoluteY)
                    outputHeight = static_cast<UINT>(shaderPass.m_shader.m_scaleY);
                else
                    outputHeight = static_cast<UINT>(sourceHeight * shaderPass.m_shader.m_scaleY);
                passSizes.push_back({sourceWidth, sourceHeight, outputWidth, outputHeight});
                if(!shaderPass.m_shader.m_alias.empty())
                {
                    m_textureSizes.insert(std::make_pair(shaderPass.m_shader.m_alias, float4 {(float)outputWidth, (float)outputHeight, 1.0f / outputWidth, 1.0f / outputHeight}));
                }
                sourceWidth  = outputWidth;
                sourceHeight = outputHeight;
            }
        }

        // call resize once all textureSizes are determined
        for(int p = 0; p < m_shaderPasses.size(); p++)
        {
            auto& shaderPass = m_shaderPasses[p];
            shaderPass.Resize(passSizes[p][0], passSizes[p][1], passSizes[p][2], passSizes[p][3], m_textureSizes, passSizes);
        }
    }

    if(rebuildPasses)
    {
        DestroyPasses();

        for(auto& pt : m_presetTextures)
        {
            // re-add static preset textures
            m_passResources.insert(pt);
        }

        m_passResources.insert(std::make_pair("Original", m_originalView));

        m_preprocessPass.m_targetView = m_preprocessedRenderTarget.get();
        if(m_shaderPasses.size() > 1)
        {
            D3D11_TEXTURE2D_DESC desc2 = {};
            texture->GetDesc(&desc2);
            desc2.Usage          = D3D11_USAGE_DEFAULT;
            desc2.BindFlags      = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
            desc2.CPUAccessFlags = 0;
            desc2.MiscFlags      = 0;

            for(const auto& pass : m_shaderPasses)
            {
                m_requiresFeedback |= pass.RequiresFeedback();
                m_requiresHistory = max(m_requiresHistory, pass.RequiresHistory());
            }

            for(size_t p = 1; p < m_shaderPasses.size(); p++)
            {
                const auto& pass = m_shaderPasses[p - 1];

                desc2.Format = pass.m_shader.m_format;

                // use shader output size
                desc2.Width     = pass.m_destWidth;
                desc2.Height    = pass.m_destHeight;
                desc2.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

                winrt::com_ptr<ID3D11Texture2D> passTexture;
                hr = m_device->CreateTexture2D(&desc2, nullptr, passTexture.put());
                assert(SUCCEEDED(hr));
                m_passTextures.push_back(passTexture);

                winrt::com_ptr<ID3D11RenderTargetView> passTarget;
                hr = m_device->CreateRenderTargetView(passTexture.get(), nullptr, passTarget.put());
                assert(SUCCEEDED(hr));
                m_passTargets.push_back(passTarget);

                winrt::com_ptr<ID3D11ShaderResourceView> passResource;
                hr = m_device->CreateShaderResourceView(passTexture.get(), nullptr, passResource.put());
                assert(SUCCEEDED(hr));
                m_passResources.insert(std::make_pair(std::string("PassOutput") + std::to_string(p - 1), passResource));
                if(!pass.m_shader.m_alias.empty())
                {
                    m_passResources.insert(std::make_pair(pass.m_shader.m_alias, passResource));
                }

                // create feedback textures if needed
                if(m_requiresFeedback)
                {
                    desc2.BindFlags = D3D11_BIND_SHADER_RESOURCE;

                    winrt::com_ptr<ID3D11Texture2D> feedbackTexture;
                    hr = m_device->CreateTexture2D(&desc2, nullptr, feedbackTexture.put());
                    assert(SUCCEEDED(hr));
                    m_passTextures.push_back(feedbackTexture);
                    winrt::com_ptr<ID3D11ShaderResourceView> feedbackResource;
                    hr = m_device->CreateShaderResourceView(feedbackTexture.get(), nullptr, feedbackResource.put());
                    assert(SUCCEEDED(hr));
                    m_passResources.insert(std::make_pair(std::string("PassFeedback") + std::to_string(p - 1), feedbackResource));
                    if(!pass.m_shader.m_alias.empty())
                    {
                        m_passResources.insert(std::make_pair(pass.m_shader.m_alias + "Feedback", feedbackResource));
                    }
                }

                m_shaderPasses[p - 1].m_targetView = passTarget.get();
                m_shaderPasses[p].m_sourceView     = passResource.get();
            }
        }
        else
        {
            m_requiresHistory = m_shaderPasses.begin()->RequiresHistory();
        }

        if(m_requiresHistory)
        {
            D3D11_TEXTURE2D_DESC desc2 = {};
            texture->GetDesc(&desc2);
            desc2.Usage          = D3D11_USAGE_DEFAULT;
            desc2.BindFlags      = D3D11_BIND_SHADER_RESOURCE;
            desc2.CPUAccessFlags = 0;
            desc2.MiscFlags      = 0;
            desc2.Width          = m_vertical ? originalHeight : originalWidth;
            desc2.Height         = m_vertical ? originalWidth : originalHeight;

            for(int h = 0; h < m_requiresHistory; h++)
            {
                winrt::com_ptr<ID3D11Texture2D> historyTexture;
                hr = m_device->CreateTexture2D(&desc2, nullptr, historyTexture.put());
                assert(SUCCEEDED(hr));
                m_passTextures.push_back(historyTexture);
                winrt::com_ptr<ID3D11ShaderResourceView> historyResource;
                hr = m_device->CreateShaderResourceView(historyTexture.get(), nullptr, historyResource.put());
                assert(SUCCEEDED(hr));
                m_passResources.insert(std::make_pair(std::string("OriginalHistory") + std::to_string(h + 1), historyResource));
            }
        }

        m_shaderPasses[m_shaderPasses.size() - 1].m_targetView = m_displayRenderTarget.get();

        if(m_requiresFeedback)
        {
            // add feedback for last pass
            int         p        = (int)m_shaderPasses.size() - 1;
            const auto& lastPass = m_shaderPasses[p];

            D3D11_TEXTURE2D_DESC desc2 = {};
            m_displayTexture->GetDesc(&desc2);
            desc2.Usage          = D3D11_USAGE_DEFAULT;
            desc2.BindFlags      = D3D11_BIND_SHADER_RESOURCE;
            desc2.CPUAccessFlags = 0;
            desc2.MiscFlags      = 0;
            desc2.Width          = lastPass.m_destWidth;
            desc2.Height         = lastPass.m_destHeight;

            winrt::com_ptr<ID3D11Texture2D> feedbackTexture;
            hr = m_device->CreateTexture2D(&desc2, nullptr, feedbackTexture.put());
            assert(SUCCEEDED(hr));
            m_passTextures.push_back(feedbackTexture);
            winrt::com_ptr<ID3D11ShaderResourceView> feedbackResource;
            hr = m_device->CreateShaderResourceView(feedbackTexture.get(), nullptr, feedbackResource.put());
            assert(SUCCEEDED(hr));
            m_passResources.insert(std::make_pair(std::string("PassFeedback") + std::to_string(p), feedbackResource));
            if(!lastPass.m_shader.m_alias.empty())
            {
                m_passResources.insert(std::make_pair(lastPass.m_shader.m_alias + "Feedback", feedbackResource));
            }
        }
    }

    if(outputMoved || outputResized || inputResized || (m_lastPos.x != topLeft.x || m_lastPos.y != topLeft.y) || m_lockedAreaUpdated)
    {
        // preprocess captured frame to a texture: crop (via scale & translation), reduce resolution, and whatnot (invert y?)
        float sx = 1.0f, sy = 1.0f, tx = 0.0f, ty = 0.0f;
        POINT finalTopLeft  = topLeft;
        m_lockedAreaUpdated = false;
        if(m_lockedArea.right - m_lockedArea.left != 0)
        {
            // we only lock position
            finalTopLeft.x = m_lockedArea.left;
            finalTopLeft.y = m_lockedArea.top;
        }
        if(!m_captureWindow && !m_image)
        {
            if(m_clone)
            {
                // desktop clone (take from 0,0)
                auto clientW = destWidth;
                auto clientH = destHeight;
                sx           = 1.0f * capturedTextureDesc.Width / clientW;
                sy           = 1.0f * capturedTextureDesc.Height / clientH;
                tx           = 1.0f * capturedTextureDesc.Width / clientW - 1.0f;
                ty           = -1.0f * capturedTextureDesc.Height / clientH + 1.0f;
            }
            else
            {
                // desktop glass
                auto clientW = destWidth;
                auto clientH = destHeight;
                sx           = 1.0f * capturedTextureDesc.Width / clientW;
                sy           = 1.0f * capturedTextureDesc.Height / clientH;
                tx           = -(2.0f * finalTopLeft.x - capturedTextureDesc.Width) / clientW - 1.0f;
                ty           = (2.0f * finalTopLeft.y - capturedTextureDesc.Height) / clientH + 1.0f;
            }
        }
        else
        {
            if(m_clone)
            {
                auto clientW = destWidth;
                auto clientH = destHeight;
                if(m_freeScale)
                {
                    clientW = captureClient.right;
                    clientH = captureClient.bottom;
                }
                sx = 1.0f * capturedTextureDesc.Width / clientW;
                sy = 1.0f * capturedTextureDesc.Height / clientH;
                tx = -(2.0f * (captureTopLeft.x - captureRect.left) - capturedTextureDesc.Width) / clientW - 1.0f;
                ty = (2.0f * (captureTopLeft.y - captureRect.top) - capturedTextureDesc.Height) / clientH + 1.0f;
            }
            else
            {
                // window glass
                auto clientW = destWidth;
                auto clientH = destHeight;
                sx           = 1.0f * capturedTextureDesc.Width / clientW;
                sy           = 1.0f * capturedTextureDesc.Height / clientH;
                tx           = -(2.0f * (finalTopLeft.x - captureRect.left) - capturedTextureDesc.Width) / clientW - 1.0f;
                ty           = (2.0f * (finalTopLeft.y - captureRect.top) - capturedTextureDesc.Height) / clientH + 1.0f;
            }
        }
        if(m_flipHorizontal)
        {
            sx *= -1.0f;
            tx *= -1.0f;
        }
        if(m_flipVertical)
        {
            sy *= -1.0f;
            ty *= -1.0f;
        }

        // offset to move away from edges; needed for SG to consistently pick up n-th input pixel if asked to, but I should find a formula to calculate this
        tx += 0.0001f;
        ty += 0.0001f;

        m_preprocessPass.UpdateMVP(sx, sy, tx, ty);
        m_lastPos.x = topLeft.x;
        m_lastPos.y = topLeft.y;
    }

    if(m_captureWindow && !m_clone)
    {
        // clear any blanks around captured window
        m_context->ClearRenderTargetView(m_preprocessedRenderTarget.get(), background_colour);
    }

    winrt::com_ptr<ID3D11ShaderResourceView> textureView;
    hr = m_device->CreateShaderResourceView(texture.get(), nullptr, textureView.put());
    assert(SUCCEEDED(hr));
    m_preprocessPass.Render(textureView.get(), m_passResources, logicalFrameNo, 0, 0);

    if(m_cursorEmulator.Hidden())
    {
        CURSORINFO ci {.cbSize = sizeof(CURSORINFO)};
        if(GetCursorInfo(&ci))
        {
            auto mx = ci.ptScreenPos.x;
            auto my = ci.ptScreenPos.y;
            mx -= topLeft.x;
            my -= topLeft.y;

            auto cursor = m_cursorEmulator.GetCursor();
            if(cursor && cursor->image)
            {
                mx -= cursor->hotSpotX;
                my -= cursor->hotSpotY;

                float cx, cy, cw, ch;

                if(m_vertical)
                {
                    cx = m_preprocessPass.m_destWidth - (my + cursor->w) / m_inputScaleW;
                    cy = m_preprocessPass.m_destHeight - (mx + cursor->h) / m_inputScaleH;
                    cw = cursor->w / m_inputScaleW;
                    ch = cursor->h / m_inputScaleH;
                }
                else
                {
                    cx = mx / m_inputScaleW;
                    cy = my / m_inputScaleH;
                    cw = cursor->w / m_inputScaleW;
                    ch = cursor->h / m_inputScaleH;
                }
                m_preprocessPass.RenderCursor(cx, cy, cw, ch, cursor->view);
            }
        }
    }

    int p = 0;
    for(auto& shaderPass : m_shaderPasses)
    {
        auto passBoxX = p == m_shaderPasses.size() - 1 ? m_boxX : 0;
        auto passBoxY = p == m_shaderPasses.size() - 1 ? m_boxY : 0;

        if(p == 0)
        {
            shaderPass.Render(m_originalView.get(), m_passResources, logicalFrameNo, passBoxX, passBoxY);
        }
        else
        {
            shaderPass.Render(m_passResources, logicalFrameNo, passBoxX, passBoxY);
        }
        p++;
    }

    if(m_requiresFeedback)
    {
        // copy output to feedback
        for(size_t q = 0; q < m_shaderPasses.size() - 1; q++)
        {
            auto                           passOutput   = m_passResources.find(std::string("PassOutput") + std::to_string(q));
            auto                           passFeedback = m_passResources.find(std::string("PassFeedback") + std::to_string(q));
            winrt::com_ptr<ID3D11Resource> outputResource;
            winrt::com_ptr<ID3D11Resource> feedbackResource;
            passOutput->second->GetResource(outputResource.put());
            passFeedback->second->GetResource(feedbackResource.put());
            m_context->CopyResource(feedbackResource.get(), outputResource.get());
        }

        // copy display texture as last pass feedback
        auto displayTexture = m_displayTexture;
        if(displayTexture)
        {
            int                            p                = (int)m_shaderPasses.size() - 1;
            const auto&                    lastPass         = m_shaderPasses[p];
            auto                           lastPassFeedback = m_passResources.find(std::string("PassFeedback") + std::to_string(p));
            winrt::com_ptr<ID3D11Resource> lastPassFeedbackResource;
            lastPassFeedback->second->GetResource(lastPassFeedbackResource.put());
            D3D11_TEXTURE2D_DESC desc3 = {};
            displayTexture->GetDesc(&desc3);
            if(m_boxX != 0 || m_boxY != 0 || lastPass.m_destWidth != desc3.Width || lastPass.m_destHeight != desc3.Height)
            {
                D3D11_BOX srcBox;
                srcBox.left   = m_boxX;
                srcBox.right  = srcBox.left + lastPass.m_destWidth;
                srcBox.top    = m_boxY;
                srcBox.bottom = srcBox.top + lastPass.m_destHeight;
                srcBox.back   = 1;
                srcBox.front  = 0;
                m_context->CopySubresourceRegion(lastPassFeedbackResource.get(), 0, 0, 0, 0, displayTexture.get(), 0, &srcBox);
            }
            else
            {
                m_context->CopyResource(lastPassFeedbackResource.get(), displayTexture.get());
            }
        }
    }

    if(m_requiresHistory)
    {
        // lookup oldest History for reuse
        const auto&                    lastHistory     = m_passResources.find(std::string("OriginalHistory" + std::to_string(m_requiresHistory)));
        auto                           lastHistoryView = lastHistory->second;
        winrt::com_ptr<ID3D11Resource> lastHistoryResource;
        lastHistoryView->GetResource(lastHistoryResource.put());

        for(int h = m_requiresHistory; h > 1; h--)
        {
            // remap middle Histories one frame back
            m_passResources[std::string("OriginalHistory" + std::to_string(h))] = m_passResources[std::string("OriginalHistory" + std::to_string(h - 1))];
        }

        // copy current Original to History1 for next pass
        const auto&                    original = m_passResources.find("Original");
        winrt::com_ptr<ID3D11Resource> originalResource;
        original->second->GetResource(originalResource.put());

        m_context->CopyResource(lastHistoryResource.get(), originalResource.get());
        if(m_requiresHistory > 1)
        {
            m_passResources["OriginalHistory1"] = lastHistoryView;
        }
    }

    PresentFrame();

    m_renderCounter++;
    m_prevRenderTicks = GetTickCount64();
    if(m_prevRenderTicks - m_prevTicks > 1000)
    {
        auto deltaTicks     = m_prevRenderTicks - m_prevTicks;
        auto deltaFrames    = m_renderCounter - m_prevRenderCounter;
        m_fps               = deltaFrames * 1000.0f / deltaTicks;
        m_prevRenderCounter = m_renderCounter;
        m_prevTicks         = m_prevRenderTicks;
    }
}

winrt::com_ptr<ID3D11Texture2D> ShaderGlass::GrabOutput()
{
    auto                            displayTexture = m_displayTexture;
    winrt::com_ptr<ID3D11Texture2D> outputTexture;

    if(displayTexture)
    {
        D3D11_TEXTURE2D_DESC desc2 = {};
        displayTexture->GetDesc(&desc2);
        desc2.Usage          = D3D11_USAGE_DEFAULT;
        desc2.CPUAccessFlags = 0;
        desc2.MiscFlags      = 0;
        auto displayWidth    = desc2.Width;
        auto displayHeight   = desc2.Height;

        if(m_shaderPasses.size() && (m_boxX != 0 || m_boxY != 0))
        {
            const auto& lastPass = m_shaderPasses.rbegin();
            desc2.Width          = lastPass->m_destWidth;
            desc2.Height         = lastPass->m_destHeight;
            hr                   = m_device->CreateTexture2D(&desc2, nullptr, outputTexture.put());
            assert(SUCCEEDED(hr));

            D3D11_BOX srcBox;
            srcBox.left   = m_boxX;
            srcBox.right  = srcBox.left + lastPass->m_destWidth;
            srcBox.top    = m_boxY;
            srcBox.bottom = srcBox.top + lastPass->m_destHeight;
            srcBox.back   = 1;
            srcBox.front  = 0;
            // fractions :/
            if(srcBox.right > displayWidth)
            {
                auto adj = srcBox.right - displayWidth;
                srcBox.left -= adj;
                srcBox.right -= adj;
            }
            if(srcBox.bottom > displayHeight)
            {
                auto adj = srcBox.bottom - displayHeight;
                srcBox.top -= adj;
                srcBox.bottom -= adj;
            }
            m_context->CopySubresourceRegion(outputTexture.get(), 0, 0, 0, 0, displayTexture.get(), 0, &srcBox);
        }
        else
        {
            hr = m_device->CreateTexture2D(&desc2, nullptr, outputTexture.put());
            assert(SUCCEEDED(hr));

            m_context->CopyResource(outputTexture.get(), displayTexture.get());
        }
    }
    return outputTexture;
}

void ShaderGlass::Stop()
{
    m_running = false;
}
