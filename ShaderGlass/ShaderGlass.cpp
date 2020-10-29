#include "pch.h"
#include "ShaderGlass.h"
#include "ShaderList.h"

static HRESULT hr;

ShaderGlass::ShaderGlass() : m_lastSize {}, m_lastPos {}, m_lastCaptureWindowPos {}
{
    m_shaderPreset = new Preset(new PassthroughPresetDef());
}

ShaderGlass::~ShaderGlass()
{
    std::unique_lock lock(m_mutex);

    DestroyShaders();
    delete m_preprocessPass;
    DestroyPasses();
    m_rasterizerState->Release();
    delete m_preprocessShader;
    DestroyTargets();
    m_displayRenderTarget->Release();
    m_swapChain->Release();
    m_context->Flush();
    m_context->Release();
}

void ShaderGlass::Initialize(HWND outputWindow, HWND captureWindow, bool clone, ID3D11Device* device)
{
    m_outputWindow  = outputWindow;
    m_captureWindow = captureWindow;
    m_clone         = clone;
    m_device        = device;
    m_device->GetImmediateContext(&m_context);

    // remember initial size
    m_lastPos.x = 0;
    m_lastPos.y = 0;
    RECT clientRect;
    GetClientRect(m_outputWindow, &clientRect);
    m_lastSize.x = clientRect.right;
    m_lastSize.y = clientRect.bottom;

    // create swapchain
    {
        IDXGIFactory2* dxgiFactory;
        {
            IDXGIDevice1* dxgiDevice;
            hr = m_device->QueryInterface(__uuidof(IDXGIDevice1), (void**)&dxgiDevice);
            assert(SUCCEEDED(hr));

            IDXGIAdapter* dxgiAdapter;
            hr = dxgiDevice->GetAdapter(&dxgiAdapter);
            assert(SUCCEEDED(hr));
            dxgiDevice->Release();

            DXGI_ADAPTER_DESC adapterDesc;
            dxgiAdapter->GetDesc(&adapterDesc);

            OutputDebugStringA("Graphics Device: ");
            OutputDebugStringW(adapterDesc.Description);

            hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), (void**)&dxgiFactory);
            assert(SUCCEEDED(hr));
            dxgiAdapter->Release();
        }

        DXGI_SWAP_CHAIN_DESC1 d3d11SwapChainDesc = {};
        d3d11SwapChainDesc.Width                 = 0;
        d3d11SwapChainDesc.Height                = 0;
        d3d11SwapChainDesc.Format                = DXGI_FORMAT_B8G8R8A8_UNORM;
        d3d11SwapChainDesc.SampleDesc.Count      = 1;
        d3d11SwapChainDesc.SampleDesc.Quality    = 0;
        d3d11SwapChainDesc.BufferUsage           = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        d3d11SwapChainDesc.BufferCount           = 2;
        // flip mode has a weird bug where the first frame doesn't align with the window client area, until window is moved :(
        //d3d11SwapChainDesc.Scaling               = DXGI_SCALING_NONE;
        //d3d11SwapChainDesc.SwapEffect            = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        d3d11SwapChainDesc.Scaling    = DXGI_SCALING_STRETCH;
        d3d11SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        d3d11SwapChainDesc.AlphaMode  = DXGI_ALPHA_MODE_UNSPECIFIED;
        d3d11SwapChainDesc.Flags      = 0;

        hr = dxgiFactory->CreateSwapChainForHwnd(m_device, m_outputWindow, &d3d11SwapChainDesc, 0, 0, &m_swapChain);
        assert(SUCCEEDED(hr));

        dxgiFactory->Release();
    }

    ID3D11Texture2D* framebuffer {nullptr};
    hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&framebuffer);
    assert(SUCCEEDED(hr));
    if(!framebuffer)
        throw std::exception("Unable to create framebuffer");

    m_device->CreateRenderTargetView(framebuffer, 0, &m_displayRenderTarget);
    framebuffer->Release();

    D3D11_RASTERIZER_DESC desc = {};
    desc.CullMode              = D3D11_CULL_NONE;
    desc.FillMode              = D3D11_FILL_SOLID;
    desc.DepthClipEnable       = FALSE;
    desc.MultisampleEnable     = FALSE;
    hr                         = m_device->CreateRasterizerState(&desc, &m_rasterizerState);
    assert(SUCCEEDED(hr));

    m_context->RSSetState(m_rasterizerState);

    m_preprocessShader = new Shader(new PreprocessShaderDef());
    m_preprocessShader->Create(m_device);
    m_preprocessPass = new ShaderPass(m_device, m_context, m_preprocessShader, nullptr);
    m_preprocessPass->Initialize();

    RebuildShaders();

    m_running = true;
}

void ShaderGlass::RebuildShaders()
{
    m_shaderPreset->Create(m_device);
    for(auto& shader : m_shaderPreset->m_shaders)
    {
        auto shaderPass = new ShaderPass(m_device, m_context, shader, m_shaderPreset);
        shaderPass->Initialize();
        m_shaderPasses.emplace_back(shaderPass);
    }

    m_presetTextures.clear();
    for(auto& texture : m_shaderPreset->m_textures)
    {
        m_presetTextures.insert(make_pair(texture.second->m_name, texture.second->m_textureView));
    }
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

void ShaderGlass::SetShaderPreset(PresetDef* p)
{
    auto oldShaderPreset = m_newShaderPreset;
    m_newShaderPreset    = new Preset(p);
    if(oldShaderPreset)
    {
        delete oldShaderPreset;
    }
}

void ShaderGlass::SetFrameSkip(int s)
{
    m_frameSkip = s;
}

void ShaderGlass::DestroyTargets()
{
    if(m_preprocessedRenderTarget != nullptr)
    {
        m_preprocessedRenderTarget->Release();
        m_preprocessedRenderTarget = nullptr;
        m_originalView->Release();
        m_originalView = nullptr;
        m_preprocessedTexture->Release();
        m_preprocessedTexture = nullptr;
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

        m_context->OMSetRenderTargets(0, 0, 0);
        m_displayRenderTarget->Release();

        hr = m_swapChain->ResizeBuffers(
            0, static_cast<UINT>(clientRect.right), static_cast<UINT>(clientRect.bottom), DXGI_FORMAT_UNKNOWN, 0);
        assert(SUCCEEDED(hr));

        ID3D11Texture2D* frameBuffer;
        hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&frameBuffer);
        assert(SUCCEEDED(hr));

        hr = m_device->CreateRenderTargetView(frameBuffer, NULL, &m_displayRenderTarget);
        assert(SUCCEEDED(hr));
        frameBuffer->Release();

        return true;
    }
    return false;
}

void ShaderGlass::DestroyShaders()
{
    for(auto& p : m_shaderPasses)
        delete p;
    m_shaderPasses.clear();
    delete m_shaderPreset;
}

void ShaderGlass::DestroyPasses()
{
    for(auto& rs : m_passResources)
    {
        if(rs.first.starts_with("PassOutput") || rs.first.starts_with("PassFeedback"))
        {
            rs.second->Release();
            rs.second = nullptr;
        }
    }
    for(auto& tg : m_passTargets)
        tg->Release();
    m_passTargets.clear();

    for(auto& tx : m_passTextures)
        tx->Release();
    m_passTextures.clear();

    m_passResources.clear();
    m_requiresFeedback = false;
}

void ShaderGlass::Process(ID3D11Texture2D* texture)
{
    if(!m_running || (m_frameSkip != 0 && m_frameCounter++ % m_frameSkip != 0))
    {
        // skip frame
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

    RECT clientRect;
    GetClientRect(m_outputWindow, &clientRect);

    RECT outputRect;
    GetWindowRect(m_outputWindow, &outputRect);

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
        outputMoved = (m_lastCaptureWindowPos.y != captureRect.left || m_lastCaptureWindowPos.y != captureRect.bottom);
        if(outputMoved)
        {
            m_lastCaptureWindowPos.x = captureRect.left;
            m_lastCaptureWindowPos.y = captureRect.bottom;
        }
    }

    D3D11_TEXTURE2D_DESC desc = {};
    texture->GetDesc(&desc);

    RECT textureRect;
    textureRect.right  = desc.Width;
    textureRect.bottom = desc.Height;

    auto outputResized = false;
    outputResized      = TryResizeSwapChain(clientRect, m_outputRescaled);

    // final window/viewport size
    const UINT viewportWidth  = static_cast<UINT>(clientRect.right);
    const UINT viewportHeight = static_cast<UINT>(clientRect.bottom);

    auto destWidth  = static_cast<long>(clientRect.right * m_outputScaleW);
    auto destHeight = static_cast<long>(clientRect.bottom * m_outputScaleH);

    if(destWidth == 0 || destHeight == 0)
        return;

    bool inputRescaled = m_inputRescaled;
    m_inputRescaled    = false;
    m_outputRescaled   = false;

    // force recreate
    if(inputRescaled && m_preprocessedRenderTarget != nullptr)
    {
        DestroyTargets();
    }

    bool rebuildPasses = false;

    auto newShaderPreset = m_newShaderPreset;
    if(newShaderPreset)
    {
        DestroyShaders();
        m_shaderPreset    = newShaderPreset;
        m_newShaderPreset = nullptr;
        RebuildShaders();
        inputRescaled = true;
        outputResized = true;
        rebuildPasses = true;
    }

    // size of preprocessed input, which is 'original' for the shader chain
    const UINT originalWidth  = static_cast<UINT>(destWidth / m_inputScaleW);
    const UINT originalHeight = static_cast<UINT>(destHeight / m_inputScaleH);

    // create preprocessed output texture, scaled down size, inverted etc.
    if(m_preprocessedTexture == nullptr)
    {
        D3D11_TEXTURE2D_DESC desc2 = {};
        texture->GetDesc(&desc2);
        desc2.Usage          = D3D11_USAGE_DEFAULT;
        desc2.BindFlags      = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
        desc2.CPUAccessFlags = 0;
        desc2.MiscFlags      = 0;
        desc2.Width          = originalWidth;
        desc2.Height         = originalHeight;

        hr = m_device->CreateTexture2D(&desc2, nullptr, &m_preprocessedTexture);
        assert(SUCCEEDED(hr));
        outputResized = true;
        rebuildPasses = true;

        hr = m_device->CreateShaderResourceView(m_preprocessedTexture, nullptr, &m_originalView);
        assert(SUCCEEDED(hr));
    }

    // create texture render target
    if(m_preprocessedRenderTarget == nullptr)
    {
        hr = m_device->CreateRenderTargetView(m_preprocessedTexture, NULL, &m_preprocessedRenderTarget);
        assert(SUCCEEDED(hr));
        rebuildPasses = true;
    }

    if(inputRescaled || outputResized)
    {
        m_textureSizes.clear();
        m_textureSizes.insert(
            std::make_pair("Original", float4 {(float)originalWidth, (float)originalHeight, 1.0f / originalWidth, 1.0f / originalHeight}));
        m_textureSizes.insert(std::make_pair(
            "FinalViewport", float4 {(float)viewportWidth, (float)viewportHeight, 1.0f / viewportWidth, 1.0f / viewportHeight}));

        // preprocess takes original texture full size
        m_preprocessPass->Resize(desc.Width, desc.Height, originalWidth, originalHeight, m_textureSizes);

        UINT                             sourceWidth  = originalWidth;
        UINT                             sourceHeight = originalHeight;
        std::vector<std::array<UINT, 4>> passSizes;
        for(int p = 0; p < m_shaderPasses.size(); p++)
        {
            auto& shaderPass = m_shaderPasses[p];
            if(p == m_shaderPasses.size() - 1) // last shader scales source to viewport
                passSizes.push_back({sourceWidth, sourceHeight, viewportWidth, viewportHeight});
            else
            {
                UINT outputWidth  = sourceWidth;
                UINT outputHeight = sourceHeight;
                if(shaderPass->m_shader->m_scaleX != 1.0f)
                {
                    if(shaderPass->m_shader->m_scaleViewportX)
                        outputWidth = static_cast<UINT>(viewportWidth * shaderPass->m_shader->m_scaleX);
                    else if(shaderPass->m_shader->m_scaleAbsoluteX)
                        outputWidth = static_cast<UINT>(shaderPass->m_shader->m_scaleX);
                    else
                        outputWidth = static_cast<UINT>(sourceWidth * shaderPass->m_shader->m_scaleX);
                }
                if(shaderPass->m_shader->m_scaleY != 1.0f)
                {
                    if(shaderPass->m_shader->m_scaleViewportY)
                        outputHeight = static_cast<UINT>(viewportHeight * shaderPass->m_shader->m_scaleY);
                    else if(shaderPass->m_shader->m_scaleAbsoluteY)
                        outputHeight = static_cast<UINT>(shaderPass->m_shader->m_scaleY);
                    else
                        outputHeight = static_cast<UINT>(sourceHeight * shaderPass->m_shader->m_scaleY);
                }
                passSizes.push_back({sourceWidth, sourceHeight, outputWidth, outputHeight});
                if(!shaderPass->m_shader->m_alias.empty())
                {
                    m_textureSizes.insert(
                        std::make_pair(shaderPass->m_shader->m_alias,
                                       float4 {(float)outputWidth, (float)outputHeight, 1.0f / outputWidth, 1.0f / outputHeight}));
                }
                sourceWidth  = outputWidth;
                sourceHeight = outputHeight;
            }
        }

        // call resize once all textureSizes are determined
        for(int p = 0; p < m_shaderPasses.size(); p++)
        {
            auto& shaderPass = m_shaderPasses[p];
            shaderPass->Resize(passSizes[p][0], passSizes[p][1], passSizes[p][2], passSizes[p][3], m_textureSizes);
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

        m_preprocessPass->m_targetView = m_preprocessedRenderTarget;
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
                m_requiresFeedback |= pass->RequiresFeedback();
            }

            for(size_t p = 1; p < m_shaderPasses.size(); p++)
            {
                const auto& pass = m_shaderPasses[p - 1];

                if(pass->m_shader->m_formatFloat)
                    desc2.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
                else if(pass->m_shader->m_formatSRGB)
                    desc2.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
                else
                    desc2.Format = DXGI_FORMAT_B8G8R8A8_UNORM;

                // use shader output size
                desc2.Width  = pass->m_destWidth;
                desc2.Height = pass->m_destHeight;

                ID3D11Texture2D* passTexture;
                hr = m_device->CreateTexture2D(&desc2, nullptr, &passTexture);
                assert(SUCCEEDED(hr));
                m_passTextures.push_back(passTexture);

                ID3D11RenderTargetView* passTarget;
                hr = m_device->CreateRenderTargetView(passTexture, nullptr, &passTarget);
                assert(SUCCEEDED(hr));
                m_passTargets.push_back(passTarget);

                ID3D11ShaderResourceView* passResource;
                hr = m_device->CreateShaderResourceView(passTexture, nullptr, &passResource);
                assert(SUCCEEDED(hr));
                m_passResources.insert(std::make_pair(std::string("PassOutput") + std::to_string(p - 1), passResource));
                if(!pass->m_shader->m_alias.empty())
                {
                    m_passResources.insert(std::make_pair(pass->m_shader->m_alias, passResource));
                }

                // create feedback textures if needed
                if(m_requiresFeedback)
                {
                    ID3D11Texture2D* feedbackTexture;
                    hr = m_device->CreateTexture2D(&desc2, nullptr, &feedbackTexture);
                    assert(SUCCEEDED(hr));
                    m_passTextures.push_back(feedbackTexture);
                    ID3D11ShaderResourceView* feedbackResource;
                    hr = m_device->CreateShaderResourceView(feedbackTexture, nullptr, &feedbackResource);
                    assert(SUCCEEDED(hr));
                    m_passResources.insert(std::make_pair(std::string("PassFeedback") + std::to_string(p - 1), feedbackResource));
                    if(!pass->m_shader->m_alias.empty())
                    {
                        m_passResources.insert(std::make_pair(pass->m_shader->m_alias + "Feedback", feedbackResource));
                    }
                }

                m_shaderPasses[p - 1]->m_targetView = passTarget;
                m_shaderPasses[p]->m_sourceView     = passResource;
            }
        }
        m_shaderPasses[m_shaderPasses.size() - 1]->m_targetView = m_displayRenderTarget;
    }

    if(outputMoved || outputResized || (m_lastPos.x != topLeft.x || m_lastPos.y != topLeft.y))
    {
        // preprocess captured frame to a texture: crop (via scale & translation), reduce resolution, and whatnot (invert y?)
        float sx = 1.0f, sy = 1.0f, tx = 0.0f, ty = 0.0f;
        if(!m_captureWindow)
        {
            if(m_clone)
            {
                // desktop clone (take from 0,0)
                auto clientW = destWidth;
                auto clientH = destHeight;
                sx           = 1.0f * desc.Width / clientW;
                sy           = 1.0f * desc.Height / clientH;
                tx           = 1.0f * desc.Width / clientW - 1.0f;
                ty           = -1.0f * desc.Height / clientH + 1.0f;
            }
            else
            {
                // desktop glass
                auto clientW = destWidth;
                auto clientH = destHeight;
                sx           = 1.0f * desc.Width / clientW;
                sy           = 1.0f * desc.Height / clientH;
                tx           = -(2.0f * topLeft.x - desc.Width) / clientW - 1.0f;
                ty           = (2.0f * topLeft.y - desc.Height) / clientH + 1.0f;
            }
        }
        else
        {
            if(m_clone)
            {
                // window clone
                auto clientW = destWidth;
                auto clientH = destHeight;
                sx           = 1.0f * captureClient.right / clientW;
                sy           = 1.0f * captureClient.bottom / clientH;
                sx           = 1.0f * desc.Width / clientW;
                sy           = 1.0f * desc.Height / clientH;
                tx           = -(2.0f * (captureTopLeft.x - captureRect.left) - desc.Width) / clientW - 1.0f;
                ty           = (2.0f * (captureTopLeft.y - captureRect.top) - desc.Height) / clientH + 1.0f;
            }
            else
            {
                // window glass
                auto clientW = destWidth;
                auto clientH = destHeight;
                sx           = 1.0f * desc.Width / clientW;
                sy           = 1.0f * desc.Height / clientH;
                tx           = -(2.0f * (topLeft.x - captureRect.left) - desc.Width) / clientW - 1.0f;
                ty           = (2.0f * (topLeft.y - captureRect.top) - desc.Height) / clientH + 1.0f;
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

        // offset to move away from edges
        tx += 0.0001f;
        ty += 0.0001f;

        m_preprocessPass->UpdateMVP(sx, sy, tx, ty);
        m_lastPos.x = topLeft.x;
        m_lastPos.y = topLeft.y;
    }

    if(m_captureWindow && !m_clone)
    {
        // clear any blanks around captured window
        float background_colour[4] = {0, 0, 0, 1.0f};
        m_context->ClearRenderTargetView(m_preprocessedRenderTarget, background_colour);
    }

    ID3D11ShaderResourceView* textureView;
    hr = m_device->CreateShaderResourceView(texture, nullptr, &textureView);
    assert(SUCCEEDED(hr));
    m_preprocessPass->Render(textureView, m_passResources);
    textureView->Release();

    int p = 0;
    for(auto& shaderPass : m_shaderPasses)
    {
        if(p == 0)
        {
            shaderPass->Render(m_originalView, m_passResources);
        }
        else
        {
            shaderPass->Render(m_passResources);
        }
        p++;
    }

    if(m_requiresFeedback)
    {
        // copy output to feedback
        for(size_t p = 0; p < m_shaderPasses.size() - 1; p++)
        {
            const auto&     pass         = m_shaderPasses[p];
            auto            passOutput   = m_passResources.find(std::string("PassOutput") + std::to_string(p));
            auto            passFeedback = m_passResources.find(std::string("PassFeedback") + std::to_string(p));
            ID3D11Resource* outputResource;
            ID3D11Resource* feedbackResource;
            passOutput->second->GetResource(&outputResource);
            passFeedback->second->GetResource(&feedbackResource);
            m_context->CopyResource(feedbackResource, outputResource);
            outputResource->Release();
            feedbackResource->Release();
        }
    }

    DXGI_PRESENT_PARAMETERS presentParameters {};
    m_swapChain->Present1(1, 0, &presentParameters);
    PostMessage(m_outputWindow, WM_PAINT, 0, 0);

    m_context->Flush();
}

void ShaderGlass::Stop()
{
    m_running = false;
}
