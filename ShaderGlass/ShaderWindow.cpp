#include "pch.h"

#include "resource.h"
#include "ShaderWindow.h"

ShaderWindow::ShaderWindow(CaptureManager& captureManager) :
    m_captureManager(captureManager), m_captureOptions(captureManager.m_options), m_title(), m_windowClass()
{ }

void ShaderWindow::LoadProfile(const std::string& fileName)
{
    try
    {
        bool paused = m_captureManager.IsActive();
        if(paused)
            Stop();

        std::ifstream infile(fileName);
        if(!infile.good())
        {
            MessageBox(NULL,
                       convertCharArrayToLPCWSTR((std::string("Unable to find profile ") + fileName).c_str()),
                       L"ShaderGlass",
                       MB_OK | MB_ICONERROR);

            return;
        }

        std::string                                       shaderCategory;
        std::string                                       shaderName;
        std::optional<std::string>                        windowName;
        std::optional<std::string>                        desktopName;
        std::optional<bool>                               transparent;
        std::optional<bool>                               clone;
        std::vector<std::tuple<int, std::string, double>> params;
        while(!infile.eof())
        {
            std::string key;
            std::string value;
            infile >> key;
            infile >> std::quoted(value);
            if(key == "ProfileVersion")
            {
                if(!value.starts_with("1."))
                    return;
            }
            else if(key == "CaptureWindow")
            {
                windowName = value;
            }
            else if(key == "CaptureDesktop")
            {
                desktopName = value;
            }
            else if(key == "PixelSize")
            {
                for(const auto& p : pixelSizes)
                {
                    if(value == p.second.mnemonic)
                        SendMessage(m_mainWindow, WM_COMMAND, p.first, 0);
                }
            }
            else if(key == "AspectRatio")
            {
                bool found = false;
                for(const auto& p : aspectRatios)
                {
                    if(value == p.second.mnemonic)
                    {
                        SendMessage(m_mainWindow, WM_COMMAND, p.first, 0);
                        found = true;
                    }
                }
                if(!found)
                {
                    // set as custom
                    try
                    {
                        float customValue = std::stof(value);
                        if(customValue != 0 && !std::isnan(customValue))
                            SendMessage(m_mainWindow, WM_COMMAND, aspectRatios.rbegin()->first, customValue * CUSTOM_PARAM_SCALE);
                    }
                    catch(std::exception&)
                    {
                        // ignored
                    }
                }
            }
            else if(key == "ShaderCategory")
            {
                shaderCategory = value;
            }
            else if(key == "ShaderName")
            {
                shaderName = value;
            }
            else if(key == "FrameSkip")
            {
                for(const auto& p : frameSkips)
                {
                    if(value == p.second.mnemonic)
                        SendMessage(m_mainWindow, WM_COMMAND, p.first, 0);
                }
            }
            else if(key == "OutputScale")
            {
                for(const auto& p : outputScales)
                {
                    if(value == p.second.mnemonic)
                        SendMessage(m_mainWindow, WM_COMMAND, p.first, 0);
                }
            }
            else if(key == "FlipH")
            {
                m_captureOptions.flipHorizontal = (value == "1");
                if(m_captureOptions.flipHorizontal)
                    CheckMenuItem(m_flipMenu, IDM_FLIP_HORIZONTAL, MF_CHECKED | MF_BYCOMMAND);
                else
                    CheckMenuItem(m_flipMenu, IDM_FLIP_HORIZONTAL, MF_UNCHECKED | MF_BYCOMMAND);
            }
            else if(key == "FlipV")
            {
                m_captureOptions.flipVertical = (value == "1");
                if(m_captureOptions.flipVertical)
                    CheckMenuItem(m_flipMenu, IDM_FLIP_VERTICAL, MF_CHECKED | MF_BYCOMMAND);
                else
                    CheckMenuItem(m_flipMenu, IDM_FLIP_VERTICAL, MF_UNCHECKED | MF_BYCOMMAND);
            }
            else if(key == "Clone")
            {
                clone = (value == "1");
            }
            else if(key == "Transparent")
            {
                transparent = (value == "1");
            }
            else if(key == "CaptureCursor")
            {
                m_captureOptions.captureCursor = (value == "1");
                if(m_captureOptions.captureCursor)
                    CheckMenuItem(m_inputMenu, IDM_INPUT_CAPTURECURSOR, MF_CHECKED | MF_BYCOMMAND);
                else
                    CheckMenuItem(m_inputMenu, IDM_INPUT_CAPTURECURSOR, MF_UNCHECKED | MF_BYCOMMAND);
            }
            else if(key.starts_with("Param-") && key.size() >= 9)
            {
                try
                {
                    size_t start = 6;
                    size_t split = key.find('-', start);
                    if(split == key.npos || split == key.size() - 1 || split == start)
                        continue;

                    auto passNo = std::stoi(key.substr(start, split - start));
                    auto name   = key.substr(split + 1);
                    params.push_back(std::make_tuple(passNo, name, std::stod(value)));
                }
                catch(std::exception& e)
                {
                    // ignored
                }
            }
        }
        infile.close();

        // try to find shader
        if(shaderName.size())
        {
            const auto& presets = m_captureManager.Presets();
            for(unsigned i = 0; i < presets.size(); i++)
            {
                if(presets.at(i)->Category == shaderCategory && presets.at(i)->Name == shaderName)
                {
                    SendMessage(m_mainWindow, WM_COMMAND, WM_SHADER(i), 0);
                    break;
                }
            }
        }

        // try to find window
        if(windowName.has_value() && windowName.value().size())
        {
            SendMessage(m_mainWindow, WM_COMMAND, IDM_WINDOW_SCAN, 0);
            for(unsigned i = 0; i < m_captureWindows.size(); i++)
            {
                if(m_captureWindows.at(i).name == windowName.value())
                {
                    SendMessage(m_mainWindow, WM_COMMAND, WM_CAPTURE_WINDOW(i), 0);
                    break;
                }
            }
        }
        else if(desktopName.has_value() && desktopName.value().size())
        {
            for(unsigned i = 0; i < m_captureDisplays.size(); i++)
            {
                if(m_captureDisplays.at(i).name == desktopName.value())
                {
                    SendMessage(m_mainWindow, WM_COMMAND, WM_CAPTURE_DISPLAY(i), 0);
                    break;
                }
            }
        }

        // only now set IO modes to override defaults
        if(clone.has_value())
        {
            m_captureOptions.clone = clone.value();
            if(m_captureOptions.clone)
            {
                CheckMenuItem(m_modeMenu, IDM_MODE_CLONE, MF_CHECKED | MF_BYCOMMAND);
                CheckMenuItem(m_modeMenu, IDM_MODE_GLASS, MF_UNCHECKED | MF_BYCOMMAND);
            }
            else
            {
                CheckMenuItem(m_modeMenu, IDM_MODE_CLONE, MF_UNCHECKED | MF_BYCOMMAND);
                CheckMenuItem(m_modeMenu, IDM_MODE_GLASS, MF_CHECKED | MF_BYCOMMAND);
            }
        }

        if(transparent.has_value())
        {
            m_captureOptions.transparent = transparent.value();
            if(m_captureOptions.transparent)
            {
                CheckMenuItem(m_outputWindowMenu, IDM_WINDOW_TRANSPARENT, MF_CHECKED | MF_BYCOMMAND);
                CheckMenuItem(m_outputWindowMenu, IDM_WINDOW_SOLID, MF_UNCHECKED | MF_BYCOMMAND);
            }
            else
            {
                CheckMenuItem(m_outputWindowMenu, IDM_WINDOW_TRANSPARENT, MF_UNCHECKED | MF_BYCOMMAND);
                CheckMenuItem(m_outputWindowMenu, IDM_WINDOW_SOLID, MF_CHECKED | MF_BYCOMMAND);
            }
        }

        // load any parameters
        if(params.size())
        {
            m_captureManager.SetParams(params);
        }

        if(paused)
            SendMessage(m_mainWindow, WM_COMMAND, IDM_START, 0);
    }
    catch(std::exception& e)
    {
        MessageBox(NULL,
                   convertCharArrayToLPCWSTR((std::string("Error loading profile: ") + std::string(e.what())).c_str()),
                   L"ShaderGlass",
                   MB_OK | MB_ICONERROR);
    }
}

void ShaderWindow::LoadProfile()
{
    OPENFILENAMEW ofn;
    char          szFileName[MAX_PATH] = "";
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner   = NULL;
    ofn.lpstrFilter = (LPCWSTR)L"ShaderGlass Profiles (*.sgp)\0*.sgp\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile   = (LPWSTR)szFileName;
    ofn.nMaxFile    = MAX_PATH;
    ofn.Flags       = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = (LPCWSTR)L"sgp";

    if(GetOpenFileName(&ofn))
    {
        std::wstring ws(ofn.lpstrFile);
        LoadProfile(std::string(ws.begin(), ws.end()));
    }
}

void ShaderWindow::SaveProfile(const std::string& fileName)
{
    const auto& pixelSize   = pixelSizes.at(WM_PIXEL_SIZE(m_selectedPixelSize));
    const auto& outputScale = outputScales.at(WM_OUTPUT_SCALE(m_selectedOutputScale));
    const auto& aspectRatio = aspectRatios.at(WM_ASPECT_RATIO(m_selectedAspectRatio));
    const auto& frameSkip   = frameSkips.at(WM_FRAME_SKIP(m_selectedFrameSkip));
    const auto& shader      = m_captureManager.Presets().at(m_captureOptions.presetNo);

    std::ofstream outfile(fileName);
    outfile << "ProfileVersion " << std::quoted("1.0") << std::endl;
    outfile << "PixelSize " << std::quoted(pixelSize.mnemonic) << std::endl;
    if(aspectRatio.mnemonic == CUSTOM_MNEMONIC)
        outfile << "AspectRatio " << std::quoted(std::to_string(aspectRatio.r)) << std::endl;
    else
        outfile << "AspectRatio " << std::quoted(aspectRatio.mnemonic) << std::endl;
    outfile << "ShaderCategory " << std::quoted(shader->Category) << std::endl;
    outfile << "ShaderName " << std::quoted(shader->Name) << std::endl;
    outfile << "FrameSkip " << std::quoted(frameSkip.mnemonic) << std::endl;
    outfile << "OutputScale " << std::quoted(outputScale.mnemonic) << std::endl;
    outfile << "FlipH " << std::quoted(std::to_string(m_captureOptions.flipHorizontal)) << std::endl;
    outfile << "FlipV " << std::quoted(std::to_string(m_captureOptions.flipVertical)) << std::endl;
    outfile << "Clone " << std::quoted(std::to_string(m_captureOptions.clone)) << std::endl;
    outfile << "CaptureCursor " << std::quoted(std::to_string(m_captureOptions.clone)) << std::endl;
    outfile << "Transparent " << std::quoted(std::to_string(m_captureOptions.transparent)) << std::endl;
    if(m_captureOptions.captureWindow)
        outfile << "CaptureWindow " << std::quoted(GetWindowStringText(m_captureOptions.captureWindow)) << std::endl;
    else if(m_captureOptions.monitor)
    {
        MONITORINFOEX info;
        info.cbSize = sizeof(info);
        GetMonitorInfo(m_captureOptions.monitor, &info);
        std::wstring wname(info.szDevice);
        outfile << "CaptureDesktop " << std::quoted(std::string(wname.begin(), wname.end())) << std::endl;
    }
    for(const auto& pt : m_captureManager.Params())
    {
        const auto& s = std::get<0>(pt);
        const auto& p = std::get<1>(pt);
        if(p->currentValue != p->defaultValue)
        {
            outfile << "Param-" << s << "-" << p->name << " " << std::quoted(std::to_string(p->currentValue)) << std::endl;
        }
    }
    outfile.close();
}

void ShaderWindow::SaveProfile()
{
    OPENFILENAMEW ofn;
    char          szFileName[MAX_PATH] = "";
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner   = NULL;
    ofn.lpstrFilter = (LPCWSTR)L"ShaderGlass Profiles (*.sgp)\0*.sgp\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile   = (LPWSTR)szFileName;
    ofn.nMaxFile    = MAX_PATH;
    ofn.Flags       = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = (LPCWSTR)L"sgp";

    if(GetSaveFileName(&ofn))
    {
        std::wstring ws(ofn.lpstrFile);
        SaveProfile(std::string(ws.begin(), ws.end()));
    }
}

BOOL CALLBACK ShaderWindow::EnumDisplayMonitorsProc(_In_ HMONITOR hMonitor, _In_ HDC hDC, _In_ LPRECT lpRect, _In_ LPARAM lParam)
{
    if(m_captureDisplays.size() >= MAX_CAPTURE_DISPLAYS)
        return false;

    MONITORINFOEX info;
    info.cbSize = sizeof(info);
    GetMonitorInfo(hMonitor, &info);

    std::wstring   wname(info.szDevice);
    CaptureDisplay cd(hMonitor, std::string(wname.begin(), wname.end()));
    if(cd.name.size())
        m_captureDisplays.emplace_back(cd);

    return true;
}

BOOL CALLBACK ShaderWindow::EnumWindowsProc(_In_ HWND hwnd, _In_ LPARAM lParam)
{
    if(m_captureWindows.size() >= MAX_CAPTURE_WINDOWS)
        return false;

    if(hwnd != m_mainWindow && IsWindowVisible(hwnd) && IsAltTabWindow(hwnd))
    {
        DWORD isCloaked;
        DWORD size = sizeof(isCloaked);
        DwmGetWindowAttribute(hwnd, DWMWA_CLOAKED, &isCloaked, size);

        if(!isCloaked)
        {
            TITLEBARINFO ti;
            ti.cbSize = sizeof(ti);
            GetTitleBarInfo(hwnd, &ti);

            // commented out to find Kodi window
            /*if(ti.rgstate[0] & STATE_SYSTEM_INVISIBLE)
                return true;*/

            if(GetWindowLong(hwnd, GWL_EXSTYLE) & WS_EX_TOOLWINDOW)
                return true;

            CaptureWindow cw(hwnd, GetWindowStringText(hwnd));
            if(cw.name.size())
                m_captureWindows.emplace_back(cw);
        }
    }
    return true;
}

void ShaderWindow::ScanWindows()
{
    m_captureWindows.clear();
    EnumWindows(&ShaderWindow::EnumWindowsProcProxy, (LPARAM)this);

    for(UINT i = 0; i < MAX_CAPTURE_WINDOWS; i++)
    {
        RemoveMenu(m_windowMenu, WM_CAPTURE_WINDOW(i), MF_BYCOMMAND);
    }
    UINT i = 0;
    for(const auto& w : m_captureWindows)
    {
        AppendMenu(m_windowMenu, MF_STRING, WM_CAPTURE_WINDOW(i++), convertCharArrayToLPCWSTR(w.name.c_str()));
        if(m_captureOptions.captureWindow == w.hwnd)
            CheckMenuItem(m_windowMenu, WM_CAPTURE_WINDOW(i - 1), MF_CHECKED | MF_BYCOMMAND);
    }
}

void ShaderWindow::ScanDisplays()
{
    m_captureDisplays.clear();

    if(!Is1903())
    {
        CaptureDisplay cd(NULL, "All Displays");
        m_captureDisplays.emplace_back(cd);
    }
    else
    {
        CaptureDisplay cd(MonitorFromWindow(m_mainWindow, MONITOR_DEFAULTTOPRIMARY), "Current Display");
        m_captureDisplays.emplace_back(cd);
    }

    EnumDisplayMonitors(NULL, NULL, &ShaderWindow::EnumDisplayMonitorsProcProxy, (LPARAM)this);

    for(UINT i = 0; i < MAX_CAPTURE_DISPLAYS; i++)
    {
        RemoveMenu(m_displayMenu, WM_CAPTURE_DISPLAY(i), MF_BYCOMMAND);
    }
    UINT i = 0;
    for(const auto& w : m_captureDisplays)
    {
        AppendMenu(m_displayMenu, MF_STRING, WM_CAPTURE_DISPLAY(i++), convertCharArrayToLPCWSTR(w.name.c_str()));
        if(m_captureOptions.monitor == w.monitor)
            CheckMenuItem(m_displayMenu, WM_CAPTURE_DISPLAY(i - 1), MF_CHECKED | MF_BYCOMMAND);
    }
}

void ShaderWindow::BuildInputMenu()
{
    m_inputMenu = GetSubMenu(m_mainMenu, 1);

    RemoveMenu(GetSubMenu(m_inputMenu, 0), ID_DESKTOP_DUMMY, MF_BYCOMMAND);

    m_pixelSizeMenu = CreatePopupMenu();
    AppendMenu(m_pixelSizeMenu, MF_STRING, IDM_PIXELSIZE_NEXT, L"Next\tp");
    for(const auto& px : pixelSizes)
    {
        AppendMenu(m_pixelSizeMenu, MF_STRING, px.first, convertCharArrayToLPCWSTR(px.second.text));
    }
    InsertMenu(m_inputMenu, 3, MF_BYPOSITION | MF_STRING | MF_POPUP, (UINT_PTR)m_pixelSizeMenu, L"Pixel Size");

    m_displayMenu = GetSubMenu(m_inputMenu, 0);
    m_windowMenu  = GetSubMenu(m_inputMenu, 1);
}

void ShaderWindow::BuildOutputMenu()
{
    auto sMenu = GetSubMenu(m_mainMenu, 2);
    DeleteMenu(sMenu, 0, MF_BYPOSITION);

    m_modeMenu         = GetSubMenu(sMenu, 0);
    m_outputWindowMenu = GetSubMenu(sMenu, 1);
    m_flipMenu         = GetSubMenu(sMenu, 2);

    m_outputScaleMenu = CreatePopupMenu();
    for(const auto& os : outputScales)
    {
        AppendMenu(m_outputScaleMenu, MF_STRING, os.first, os.second.text);
    }
    AppendMenu(sMenu, MF_STRING | MF_POPUP, (UINT_PTR)m_outputScaleMenu, L"Scale");

    m_aspectRatioMenu = CreatePopupMenu();
    for(const auto& ar : aspectRatios)
    {
        AppendMenu(m_aspectRatioMenu, MF_STRING, ar.first, ar.second.text);
    }
    AppendMenu(sMenu, MF_STRING | MF_POPUP, (UINT_PTR)m_aspectRatioMenu, L"Aspect Ratio Correction");

    m_frameSkipMenu = CreatePopupMenu();
    for(const auto& fs : frameSkips)
    {
        AppendMenu(m_frameSkipMenu, MF_STRING, fs.first, fs.second.text);
    }
    AppendMenu(sMenu, MF_STRING | MF_POPUP, (UINT_PTR)m_frameSkipMenu, L"Frame Skip");

    AppendMenu(sMenu, MF_STRING, ID_PROCESSING_FULLSCREEN, L"Fullscreen\tCtrl+Shift+G");
}

void ShaderWindow::BuildShaderMenu()
{
    m_shaderMenu = GetSubMenu(m_mainMenu, 3);
    DeleteMenu(m_shaderMenu, 0, MF_BYPOSITION);
    unsigned i = 0;

    for(const auto& sp : m_captureManager.Presets())
    {
        if(strcmp(sp->Category, "general") == 0)
        {
            AppendMenu(m_shaderMenu, MF_STRING, WM_SHADER(i++), convertCharArrayToLPCWSTR(sp->Name));
            continue;
        }
        if(m_categoryMenus.find(sp->Category) == m_categoryMenus.end())
        {
            m_categoryMenus.insert(std::make_pair(sp->Category, CreatePopupMenu()));
        }
        auto menu = m_categoryMenus.find(sp->Category)->second;
        AppendMenu(menu, MF_STRING, WM_SHADER(i++), convertCharArrayToLPCWSTR(sp->Name));
    }

    for(auto m : m_categoryMenus)
    {
        AppendMenu(m_shaderMenu, MF_STRING | MF_POPUP, (UINT_PTR)m.second, convertCharArrayToLPCWSTR(m.first.c_str()));
    }
    m_numPresets = i;
}

LRESULT CALLBACK ShaderWindow::WndProcProxy(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    ShaderWindow* app;
    if(msg == WM_CREATE)
    {
        app = (ShaderWindow*)(((LPCREATESTRUCT)lParam)->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)app);
    }
    else
    {
        app = (ShaderWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    }
    return app->WndProc(hWnd, msg, wParam, lParam);
}

BOOL CALLBACK ShaderWindow::EnumWindowsProcProxy(_In_ HWND hwnd, _In_ LPARAM lParam)
{
    return ((ShaderWindow*)lParam)->EnumWindowsProc(hwnd, 0);
}

BOOL CALLBACK ShaderWindow::EnumDisplayMonitorsProcProxy(_In_ HMONITOR hMonitor, _In_ HDC hDC, _In_ LPRECT lpRect, _In_ LPARAM lParam)
{
    return ((ShaderWindow*)lParam)->EnumDisplayMonitorsProc(hMonitor, hDC, lpRect, lParam);
}

ATOM ShaderWindow::MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc   = ShaderWindow::WndProcProxy;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = 0;
    wcex.hInstance     = hInstance;
    wcex.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SHADERGLASS));
    wcex.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = CreateSolidBrush(0x00000000);
    wcex.lpszMenuName  = MAKEINTRESOURCEW(IDC_SHADERGLASS);
    wcex.lpszClassName = m_windowClass;
    wcex.hIconSm       = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    //wcex.hCursor = NULL;

    return RegisterClassExW(&wcex);
}

BOOL ShaderWindow::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    m_instance = hInstance;

    RECT rect;
    rect.left   = 0;
    rect.top    = 0;
    rect.right  = 960;
    rect.bottom = 600;
    AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, true, WS_EX_WINDOWEDGE);

    HWND hWnd = CreateWindowW(m_windowClass,
                              m_title,
                              WS_OVERLAPPEDWINDOW | WS_EX_WINDOWEDGE,
                              CW_USEDEFAULT,
                              CW_USEDEFAULT,
                              rect.right - rect.left,
                              rect.bottom - rect.top,
                              nullptr,
                              nullptr,
                              hInstance,
                              this);

    if(!hWnd)
    {
        return FALSE;
    }

    m_mainWindow = hWnd;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

void ShaderWindow::SetTransparent(bool transparent)
{
    if(transparent != m_isTransparent)
    {
        LONG cur_style = GetWindowLong(m_mainWindow, GWL_EXSTYLE);
        if(transparent)
            SetWindowLong(m_mainWindow, GWL_EXSTYLE, cur_style | WS_EX_TRANSPARENT);
        else
            SetWindowLong(m_mainWindow, GWL_EXSTYLE, cur_style & ~WS_EX_TRANSPARENT);
        m_isTransparent = transparent;
    }
}

void ShaderWindow::AdjustWindowSize(HWND hWnd)
{
    if(m_captureManager.IsActive() && m_captureOptions.captureWindow && m_captureOptions.clone)
    {
        // resize client area to captured window x scale
        RECT captureRect;
        GetClientRect(m_captureOptions.captureWindow, &captureRect);

        RECT r;
        GetClientRect(hWnd, &r);

        auto requiredW = static_cast<LONG>(captureRect.right * m_captureOptions.outputScale / m_captureOptions.aspectRatio);
        auto requiredH = static_cast<LONG>(captureRect.bottom * m_captureOptions.outputScale);
        if(requiredW == 0)
            requiredW = 1;
        if(requiredH == 0)
            requiredH = 1;

        if(r.right != requiredW || r.bottom != requiredH)
        {
            r.right  = requiredW;
            r.bottom = requiredH;
            AdjustWindowRect(&r, GetWindowLong(hWnd, GWL_STYLE), TRUE);
            SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, r.right - r.left, r.bottom - r.top, SWP_NOMOVE | SWP_NOZORDER | SWP_NOOWNERZORDER);
        }
    }
    else if(!m_isBorderless)
    {
        float xAlign = m_captureOptions.pixelWidth * m_captureOptions.outputScale;
        float yAlign = m_captureOptions.pixelHeight * m_captureOptions.outputScale;
        if(xAlign != 1 || yAlign != 1)
        {
            RECT clientRect;
            GetClientRect(hWnd, &clientRect);

            int requiredW = static_cast<LONG>(static_cast<int>(clientRect.right / xAlign) * xAlign);
            int requiredH = static_cast<LONG>(static_cast<int>(clientRect.bottom / yAlign) * yAlign);
            if(requiredW == 0)
                requiredW = 1;
            if(requiredH == 0)
                requiredH = 1;

            if(requiredW != clientRect.right || requiredH != clientRect.bottom)
            {
                clientRect.right  = requiredW;
                clientRect.bottom = requiredH;
                AdjustWindowRect(&clientRect, GetWindowLong(hWnd, GWL_STYLE), GetMenu(hWnd) != 0);

                RECT windowRect;
                GetWindowRect(hWnd, &windowRect);
                SetWindowPos(hWnd,
                             HWND_TOPMOST,
                             0,
                             0,
                             clientRect.right - clientRect.left,
                             clientRect.bottom - clientRect.top,
                             SWP_NOMOVE | SWP_NOZORDER | SWP_NOOWNERZORDER);
            }
        }
    }
}

void ShaderWindow::UpdateWindowState()
{
    // always topmost when processing
    if(m_captureManager.IsActive())
        SetWindowPos(m_mainWindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    else
        SetWindowPos(m_mainWindow, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    LONG cur_style = GetWindowLong(m_mainWindow, GWL_EXSTYLE);
    if(m_captureManager.IsActive() && m_captureOptions.transparent)
        // active desktop glass mode - layered/transparent window
        SetWindowLong(m_mainWindow, GWL_EXSTYLE, cur_style | WS_EX_LAYERED);
    else
        // clone or normal window
        SetWindowLong(m_mainWindow, GWL_EXSTYLE, cur_style & ~WS_EX_LAYERED);

    if(m_captureManager.IsActive() && !m_captureOptions.clone && !m_captureOptions.captureWindow)
    // desktop glass - exclude from capture
    {
        SetWindowDisplayAffinity(m_mainWindow, WDA_EXCLUDEFROMCAPTURE);
        //SetWindowDisplayAffinity(m_paramsWindow, WDA_EXCLUDEFROMCAPTURE);
    }
    else
    {
        SetWindowDisplayAffinity(m_mainWindow, WDA_NONE);
        //SetWindowDisplayAffinity(m_paramsWindow, WDA_NONE);
    }

    // update title
    if(m_captureManager.IsActive())
    {
        const auto& pixelSize   = pixelSizes.at(WM_PIXEL_SIZE(m_selectedPixelSize));
        const auto& outputScale = outputScales.at(WM_OUTPUT_SCALE(m_selectedOutputScale));
        const auto& aspectRatio = aspectRatios.at(WM_ASPECT_RATIO(m_selectedAspectRatio));
        const auto& shader      = m_captureManager.Presets().at(m_captureOptions.presetNo);

        char windowName[26];
        windowName[0] = 0;
        if(m_captureOptions.captureWindow)
        {
            auto captureTitle = GetWindowStringText(m_captureOptions.captureWindow);
            if(captureTitle.size())
            {
                if(captureTitle.find(',') > 0)
                {
                    captureTitle = captureTitle.substr(0, captureTitle.find(','));
                }
                if(captureTitle.size() > 20)
                {
                    captureTitle = captureTitle.substr(0, 20) + "...";
                }
                captureTitle += ", ";
                strncpy_s(windowName, captureTitle.c_str(), 26);
            }
        }

        char title[200];
        snprintf(title,
                 200,
                 "ShaderGlass (%s%s, x%s, %s%%, ~%s)",
                 windowName,
                 shader->Name,
                 pixelSize.mnemonic,
                 outputScale.mnemonic,
                 aspectRatio.mnemonic);
        SetWindowTextA(m_mainWindow, title);
    }
    else
    {
        SetWindowTextA(m_mainWindow, "ShaderGlass");
    }

    AdjustWindowSize(m_mainWindow);
}

LRESULT CALLBACK ShaderWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
    case WM_COMMAND: {
        UINT wmId = LOWORD(wParam);
        switch(wmId)
        {
        case IDM_START:
            if(!Start())
                return 0;
            break;
        case ID_PROCESSING_FULLSCREEN:
            ToggleBorderless(hWnd);
            break;
        case ID_PROCESSING_PAUSE:
            if(m_captureManager.IsActive())
                Stop();
            else
                Start();
            break;
        case ID_PROCESSING_SCREENSHOT:
            SetTimer(m_mainWindow, ID_PROCESSING_SCREENSHOT, MENU_FADE_DELAY, NULL);
            break;
        case IDM_UPDATE_PARAMS:
            PostMessage(m_paramsWindow, WM_COMMAND, IDM_UPDATE_PARAMS, 0);
            break;
        case IDM_SHADER_PARAMETERS: {
            if(!m_paramsPositioned)
            {
                RECT rc, rcDlg, rcOwner;
                GetWindowRect(m_mainWindow, &rcOwner);
                GetWindowRect(m_paramsWindow, &rcDlg);
                CopyRect(&rc, &rcOwner);
                OffsetRect(&rcDlg, -rcDlg.left, -rcDlg.top);
                OffsetRect(&rc, -rc.left, -rc.top);
                OffsetRect(&rc, -rcDlg.right, -rcDlg.bottom);

                SetWindowPos(m_paramsWindow,
                             HWND_TOP,
                             rcOwner.left + (rc.right / 2),
                             rcOwner.top + max(0, (rc.bottom / 2)),
                             0,
                             0, // Ignores size arguments.
                             SWP_NOSIZE);
                m_paramsPositioned = true;
            }
            ShowWindow(m_paramsWindow, SW_SHOW);
        }
            return 0;
        case IDM_TOGGLEMENU:
            if(GetMenu(hWnd))
                SetMenu(hWnd, NULL);
            else
                SetMenu(hWnd, m_mainMenu);
            break;
        case IDM_INPUT_CAPTURECURSOR:
            m_captureOptions.captureCursor = !m_captureOptions.captureCursor;
            m_captureManager.UpdateCursor();
            if(m_captureOptions.captureCursor)
                CheckMenuItem(m_inputMenu, IDM_INPUT_CAPTURECURSOR, MF_CHECKED | MF_BYCOMMAND);
            else
                CheckMenuItem(m_inputMenu, IDM_INPUT_CAPTURECURSOR, MF_UNCHECKED | MF_BYCOMMAND);
            break;
        case IDM_SHADER_NEXT:
            SendMessage(hWnd, WM_COMMAND, WM_SHADER((m_captureOptions.presetNo + 1) % m_numPresets), 0);
            break;
        case IDM_SHADER_RANDOM:
            SendMessage(hWnd, WM_COMMAND, WM_SHADER(rand() % m_numPresets), 0);
            break;
        case IDM_PIXELSIZE_NEXT:
            SendMessage(hWnd, WM_COMMAND, WM_PIXEL_SIZE((m_selectedPixelSize + 1) % pixelSizes.size()), 0);
            break;
        case IDM_FLIP_HORIZONTAL:
            m_captureOptions.flipHorizontal = !m_captureOptions.flipHorizontal;
            if(m_captureOptions.flipHorizontal)
                CheckMenuItem(m_flipMenu, IDM_FLIP_HORIZONTAL, MF_CHECKED | MF_BYCOMMAND);
            else
                CheckMenuItem(m_flipMenu, IDM_FLIP_HORIZONTAL, MF_UNCHECKED | MF_BYCOMMAND);
            m_captureManager.UpdateOutputFlip();
            break;
        case IDM_FLIP_VERTICAL:
            m_captureOptions.flipVertical = !m_captureOptions.flipVertical;
            if(m_captureOptions.flipVertical)
                CheckMenuItem(m_flipMenu, IDM_FLIP_VERTICAL, MF_CHECKED | MF_BYCOMMAND);
            else
                CheckMenuItem(m_flipMenu, IDM_FLIP_VERTICAL, MF_UNCHECKED | MF_BYCOMMAND);
            m_captureManager.UpdateOutputFlip();
            break;
        case IDM_WINDOW_SOLID:
            m_captureOptions.transparent = false;
            CheckMenuItem(m_outputWindowMenu, IDM_WINDOW_TRANSPARENT, MF_UNCHECKED | MF_BYCOMMAND);
            CheckMenuItem(m_outputWindowMenu, IDM_WINDOW_SOLID, MF_CHECKED | MF_BYCOMMAND);
            UpdateWindowState();
            break;
        case IDM_WINDOW_TRANSPARENT:
            m_captureOptions.transparent = true;
            CheckMenuItem(m_outputWindowMenu, IDM_WINDOW_TRANSPARENT, MF_CHECKED | MF_BYCOMMAND);
            CheckMenuItem(m_outputWindowMenu, IDM_WINDOW_SOLID, MF_UNCHECKED | MF_BYCOMMAND);
            UpdateWindowState();
            break;
        case IDM_MODE_GLASS:
            m_captureOptions.clone = false;
            CheckMenuItem(m_modeMenu, IDM_MODE_GLASS, MF_CHECKED | MF_BYCOMMAND);
            CheckMenuItem(m_modeMenu, IDM_MODE_CLONE, MF_UNCHECKED | MF_BYCOMMAND);
            m_captureOptions.transparent = true;
            CheckMenuItem(m_outputWindowMenu, IDM_WINDOW_TRANSPARENT, MF_CHECKED | MF_BYCOMMAND);
            CheckMenuItem(m_outputWindowMenu, IDM_WINDOW_SOLID, MF_UNCHECKED | MF_BYCOMMAND);
            m_captureManager.UpdateInput();
            UpdateWindowState();
            break;
        case IDM_MODE_CLONE:
            m_captureOptions.clone = true;
            CheckMenuItem(m_modeMenu, IDM_MODE_GLASS, MF_UNCHECKED | MF_BYCOMMAND);
            CheckMenuItem(m_modeMenu, IDM_MODE_CLONE, MF_CHECKED | MF_BYCOMMAND);
            m_captureOptions.transparent = false;
            CheckMenuItem(m_outputWindowMenu, IDM_WINDOW_TRANSPARENT, MF_UNCHECKED | MF_BYCOMMAND);
            CheckMenuItem(m_outputWindowMenu, IDM_WINDOW_SOLID, MF_CHECKED | MF_BYCOMMAND);
            m_captureManager.UpdateInput();
            UpdateWindowState();
            break;
        case IDM_WINDOW_SCAN:
            ScanWindows();
            break;
        /*case IDM_DISPLAY_ALLDISPLAYS:
            CheckMenuRadioItem(
                m_windowMenu, WM_CAPTURE_WINDOW(0), WM_CAPTURE_WINDOW(static_cast<UINT>(m_captureWindows.size())), 0, MF_BYCOMMAND);
            CheckMenuItem(m_displayMenu, IDM_DISPLAY_ALLDISPLAYS, MF_CHECKED | MF_BYCOMMAND);
            m_captureOptions.captureWindow = NULL;
            if(Is1903())
            {
                // switch to clone mode
                m_captureOptions.monitor     = MonitorFromWindow(hWnd, MONITOR_DEFAULTTOPRIMARY);
                m_captureOptions.clone       = true;
                m_captureOptions.transparent = false;
                CheckMenuItem(m_modeMenu, IDM_MODE_GLASS, MF_UNCHECKED | MF_BYCOMMAND);
                CheckMenuItem(m_modeMenu, IDM_MODE_CLONE, MF_CHECKED | MF_BYCOMMAND);
                CheckMenuItem(m_outputWindowMenu, IDM_WINDOW_TRANSPARENT, MF_UNCHECKED | MF_BYCOMMAND);
                CheckMenuItem(m_outputWindowMenu, IDM_WINDOW_SOLID, MF_CHECKED | MF_BYCOMMAND);
            }
            else
            {
                // switch to glass mode
                m_captureOptions.clone       = false;
                m_captureOptions.transparent = true;
                CheckMenuItem(m_modeMenu, IDM_MODE_GLASS, MF_CHECKED | MF_BYCOMMAND);
                CheckMenuItem(m_modeMenu, IDM_MODE_CLONE, MF_UNCHECKED | MF_BYCOMMAND);
                CheckMenuItem(m_outputWindowMenu, IDM_WINDOW_TRANSPARENT, MF_CHECKED | MF_BYCOMMAND);
                CheckMenuItem(m_outputWindowMenu, IDM_WINDOW_SOLID, MF_UNCHECKED | MF_BYCOMMAND);
            }
            m_captureManager.UpdateInput();
            UpdateWindowState();
            break;*/
        case IDM_STOP:
            SetTimer(m_mainWindow, IDM_STOP, MENU_FADE_DELAY, NULL);
            break;
        case IDM_PROCESSING_LOADPROFILE:
            LoadProfile();
            break;
        case IDM_PROCESSING_SAVEPROFILEAS:
            SaveProfile();
            break;
        case IDM_EXIT:
            m_captureManager.StopSession();
            DestroyWindow(hWnd);
            break;
        case IDM_ABOUT1:
        case IDM_ABOUT2:
        case IDM_ABOUT3:
#ifdef _DEBUG
            m_captureManager.Debug();
#else
            ShellExecute(0, 0, L"https://github.com/mausimus/ShaderGlass", 0, 0, SW_SHOW);
#endif
            break;
        default:
            if(wmId >= WM_USER && wmId <= 0x7FFF)
            {
                if(wmId >= WM_SHADER(0) && wmId < WM_SHADER(MAX_SHADERS))
                {
                    CheckMenuItem(m_shaderMenu, wmId, MF_CHECKED | MF_BYCOMMAND);
                    CheckMenuItem(m_shaderMenu, m_captureOptions.presetNo + WM_SHADER(0), MF_UNCHECKED | MF_BYCOMMAND);
                    m_captureOptions.presetNo = wmId - WM_SHADER(0);
                    m_captureManager.UpdateShaderPreset();
                    UpdateWindowState();
                    break;
                }
                if(wmId >= WM_CAPTURE_WINDOW(0) && wmId < WM_CAPTURE_WINDOW(MAX_CAPTURE_WINDOWS))
                {
                    CheckMenuRadioItem(m_windowMenu,
                                       WM_CAPTURE_WINDOW(0),
                                       WM_CAPTURE_WINDOW(static_cast<UINT>(m_captureWindows.size())),
                                       wmId,
                                       MF_BYCOMMAND);
                    CheckMenuRadioItem(m_displayMenu,
                                       WM_CAPTURE_DISPLAY(0),
                                       WM_CAPTURE_DISPLAY(static_cast<UINT>(m_captureDisplays.size())),
                                       0,
                                       MF_BYCOMMAND);
                    m_captureOptions.captureWindow = m_captureWindows.at(wmId - WM_CAPTURE_WINDOW(0)).hwnd;
                    m_captureOptions.monitor       = NULL;
                    m_captureOptions.clone         = true;
                    m_captureOptions.transparent   = false;
                    CheckMenuItem(m_modeMenu, IDM_MODE_GLASS, MF_UNCHECKED | MF_BYCOMMAND);
                    CheckMenuItem(m_modeMenu, IDM_MODE_CLONE, MF_CHECKED | MF_BYCOMMAND);
                    CheckMenuItem(m_outputWindowMenu, IDM_WINDOW_TRANSPARENT, MF_UNCHECKED | MF_BYCOMMAND);
                    CheckMenuItem(m_outputWindowMenu, IDM_WINDOW_SOLID, MF_CHECKED | MF_BYCOMMAND);
                    m_captureManager.UpdateInput();
                    UpdateWindowState();
                    break;
                }
                if(wmId >= WM_CAPTURE_DISPLAY(0) && wmId < WM_CAPTURE_DISPLAY(MAX_CAPTURE_DISPLAYS))
                {
                    CheckMenuRadioItem(
                        m_windowMenu, WM_CAPTURE_WINDOW(0), WM_CAPTURE_WINDOW(static_cast<UINT>(m_captureWindows.size())), 0, MF_BYCOMMAND);
                    CheckMenuRadioItem(m_displayMenu,
                                       WM_CAPTURE_DISPLAY(0),
                                       WM_CAPTURE_DISPLAY(static_cast<UINT>(m_captureDisplays.size())),
                                       wmId,
                                       MF_BYCOMMAND);
                    m_captureOptions.captureWindow = NULL;
                    m_captureOptions.monitor       = m_captureDisplays.at(wmId - WM_CAPTURE_DISPLAY(0)).monitor;
                    m_captureOptions.clone         = false;
                    m_captureOptions.transparent   = true;
                    CheckMenuItem(m_modeMenu, IDM_MODE_GLASS, MF_CHECKED | MF_BYCOMMAND);
                    CheckMenuItem(m_modeMenu, IDM_MODE_CLONE, MF_UNCHECKED | MF_BYCOMMAND);
                    CheckMenuItem(m_outputWindowMenu, IDM_WINDOW_TRANSPARENT, MF_CHECKED | MF_BYCOMMAND);
                    CheckMenuItem(m_outputWindowMenu, IDM_WINDOW_SOLID, MF_UNCHECKED | MF_BYCOMMAND);
                    m_captureManager.UpdateInput();
                    UpdateWindowState();
                    break;
                }
                const auto& pixelSize = pixelSizes.find(wmId);
                if(pixelSize != pixelSizes.end())
                {
                    m_selectedPixelSize = wmId - WM_PIXEL_SIZE(0);
                    CheckMenuRadioItem(
                        m_pixelSizeMenu, WM_PIXEL_SIZE(0), WM_PIXEL_SIZE(static_cast<UINT>(pixelSizes.size() - 1)), wmId, MF_BYCOMMAND);
                    m_captureOptions.pixelWidth  = pixelSize->second.w;
                    m_captureOptions.pixelHeight = pixelSize->second.h;
                    m_captureManager.UpdatePixelSize();
                    UpdateWindowState();
                    break;
                }
                const auto& outputScale = outputScales.find(wmId);
                if(outputScale != outputScales.end())
                {
                    m_selectedOutputScale = wmId - WM_OUTPUT_SCALE(0);
                    CheckMenuRadioItem(
                        m_outputScaleMenu, 0, static_cast<UINT>(outputScales.size()), wmId - WM_OUTPUT_SCALE(0), MF_BYPOSITION);
                    m_captureOptions.outputScale = outputScale->second.s;
                    m_captureManager.UpdateOutputSize();
                    UpdateWindowState();
                    break;
                }
                const auto& aspectRatio = aspectRatios.find(wmId);
                if(aspectRatio != aspectRatios.end())
                {
                    if(aspectRatio->second.mnemonic == CUSTOM_MNEMONIC)
                    {
                        if(lParam != 0)
                        {
                            // loading profile?
                            aspectRatio->second.r = lParam / (float)CUSTOM_PARAM_SCALE;
                        }
                        else
                        {
                            float customInput = m_inputDialog->GetInput("Aspect Ratio Correction (Pixel Height):", aspectRatio->second.r);
                            if(std::isnan(customInput))
                                break;

                            aspectRatio->second.r = customInput; // store new ratio in menu item
                        }
                    }

                    m_selectedAspectRatio = wmId - WM_ASPECT_RATIO(0);
                    CheckMenuRadioItem(
                        m_aspectRatioMenu, 0, static_cast<UINT>(aspectRatios.size()), wmId - WM_ASPECT_RATIO(0), MF_BYPOSITION);
                    m_captureOptions.aspectRatio = aspectRatio->second.r;
                    m_captureManager.UpdateOutputSize();
                    UpdateWindowState();
                    break;
                }
                const auto& frameSkip = frameSkips.find(wmId);
                if(frameSkip != frameSkips.end())
                {
                    m_selectedFrameSkip = wmId - WM_FRAME_SKIP(0);
                    CheckMenuRadioItem(m_frameSkipMenu, 0, static_cast<UINT>(frameSkips.size()), wmId - WM_FRAME_SKIP(0), MF_BYPOSITION);
                    m_captureOptions.frameSkip = frameSkip->second.s;
                    m_captureManager.UpdateFrameSkip();
                    break;
                }
            }
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_HOTKEY: {
        switch(wParam)
        {
        case HK_FULLSCREEN:
            ToggleBorderless(hWnd);
            break;
        case HK_SCREENSHOT:
            Screenshot();
            break;
        case HK_PAUSE:
            if(m_captureManager.IsActive())
                Stop();
            else
                Start();
            break;
        }

        break;
    }
    case WM_SIZE: {
        switch(wParam)
        {
        case SIZE_MINIMIZED:
            if(m_captureManager.IsActive())
            {
                m_captureOptions.paused = true;
                m_captureManager.StopSession();
            }
            break;
        case SIZE_MAXIMIZED:
        case SIZE_RESTORED:
            if(m_captureOptions.paused)
            {
                m_captureManager.StartSession();
                m_captureOptions.paused = false;
            }
            break;
        }
        //SendMessage(hWnd, WM_PRINT, (WPARAM)NULL, PRF_NONCLIENT); -- not sure what bug this was
        AdjustWindowSize(hWnd);
        return 0;
    }
    case WM_ERASEBKGND:
    case WM_SIZING: {
        // prevent flicker
        return 0;
    }
    case WM_LBUTTONDOWN: {
        if(m_captureManager.IsActive() && m_captureOptions.captureWindow)
        {
            // if we click in our client area, active the captured window
            SetForegroundWindow(m_captureOptions.captureWindow);
        }
        break;
    }
    case WM_PAINT: {
        if(m_captureManager.IsActive() && m_captureOptions.transparent)
        {
            POINT p;
            if(GetCursorPos(&p) && ScreenToClient(hWnd, &p))
            {
                RECT r;
                GetClientRect(hWnd, &r);
                if(p.x > 0 && p.x < r.right && p.y > 0 && p.y < r.bottom)
                {
                    SetTransparent(true);
                }
                else
                {
                    SetTransparent(false);
                }
            }
        }
        ValidateRect(hWnd, NULL);
        return 0;
    }
    case WM_TIMER:
        switch(wParam)
        {
        case IDM_STOP:
            KillTimer(m_mainWindow, IDM_STOP);
            Stop();
            return 0;
        case ID_PROCESSING_SCREENSHOT:
            KillTimer(m_mainWindow, ID_PROCESSING_SCREENSHOT);
            Screenshot();
            return 0;
        }
        break;
    case WM_DESTROY:
        m_captureManager.Exit();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

bool ShaderWindow::Start()
{
    if(m_captureOptions.captureWindow && !IsWindow(m_captureOptions.captureWindow))
        return false;

    if(m_captureManager.IsActive())
        return false;

    m_captureManager.StartSession();
    EnableMenuItem(m_programMenu, IDM_START, MF_BYCOMMAND | MF_DISABLED);
    EnableMenuItem(m_programMenu, IDM_STOP, MF_BYCOMMAND | MF_ENABLED);
    UpdateWindowState();

    return true;
}

void ShaderWindow::Stop()
{
    if(!m_captureManager.IsActive())
        return;

    m_captureManager.StopSession();
    EnableMenuItem(m_programMenu, IDM_STOP, MF_BYCOMMAND | MF_DISABLED);
    EnableMenuItem(m_programMenu, IDM_START, MF_BYCOMMAND | MF_ENABLED);
    UpdateWindowState();
    SendMessage(m_paramsWindow, WM_COMMAND, IDM_UPDATE_PARAMS, 0);
}

void ShaderWindow::Screenshot()
{
    m_captureManager.GrabOutput();

    OPENFILENAME ofn;
    TCHAR        szFile[260] = {0};

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize     = sizeof(ofn);
    ofn.hwndOwner       = m_mainWindow;
    ofn.lpstrFile       = szFile;
    ofn.nMaxFile        = sizeof(szFile);
    ofn.lpstrFilter     = _T("PNG\0*.png\0");
    ofn.lpstrDefExt     = _T("png");
    ofn.nFilterIndex    = 1;
    ofn.lpstrFileTitle  = NULL;
    ofn.nMaxFileTitle   = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags           = OFN_PATHMUSTEXIST;

    if(GetSaveFileName(&ofn) == TRUE)
    {
        m_captureManager.SaveOutput(ofn.lpstrFile);
    }
}

void ShaderWindow::ToggleBorderless(HWND hWnd)
{
    LONG cur_style = GetWindowLong(m_mainWindow, GWL_STYLE);
    if(!m_isBorderless)
    {
        cur_style &= ~WS_OVERLAPPEDWINDOW;
        SetMenu(hWnd, NULL);
    }
    else
    {
        cur_style |= WS_OVERLAPPEDWINDOW;
        SetMenu(hWnd, m_mainMenu);
    }
    SetWindowLong(m_mainWindow, GWL_STYLE, cur_style);

    m_isBorderless = !m_isBorderless;

    if(m_isBorderless)
    {
        RECT        clientRect;
        auto        monitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
        MONITORINFO info;
        info.cbSize = sizeof(info);
        GetMonitorInfo(monitor, &info);
        clientRect.top    = info.rcMonitor.top;
        clientRect.left   = info.rcMonitor.left;
        clientRect.right  = info.rcMonitor.right;
        clientRect.bottom = info.rcMonitor.bottom;
        AdjustWindowRect(&clientRect, GetWindowLong(hWnd, GWL_STYLE), GetMenu(hWnd) != 0);

        GetWindowRect(hWnd, &m_lastPosition);
        SetWindowPos(hWnd,
                     HWND_TOPMOST,
                     info.rcMonitor.left,
                     info.rcMonitor.top,
                     clientRect.right - clientRect.left,
                     clientRect.bottom - clientRect.top,
                     0);
    }
    else
    {
        SetWindowPos(hWnd,
                     0,
                     m_lastPosition.left,
                     m_lastPosition.top,
                     m_lastPosition.right - m_lastPosition.left,
                     m_lastPosition.bottom - m_lastPosition.top,
                     SWP_NOZORDER | SWP_NOOWNERZORDER);
    }
}

bool ShaderWindow::Create(_In_ HINSTANCE hInstance, _In_ int nCmdShow)
{
    LoadStringW(hInstance, IDS_APP_TITLE, m_title, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SHADERGLASS, m_windowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    if(!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    m_captureManager.Initialize();

    m_mainMenu = LoadMenu(hInstance, MAKEINTRESOURCEW(IDC_SHADERGLASS));

    m_programMenu = GetSubMenu(m_mainMenu, 0);
    BuildInputMenu();
    BuildOutputMenu();
    BuildShaderMenu();
    ScanWindows();
    ScanDisplays();

    if(Is1903())
    {
        ModifyMenu(GetSubMenu(m_mainMenu, 4),
                   ID_HELP_WINDOWSVERSION,
                   MF_BYCOMMAND | MF_STRING | MF_DISABLED,
                   ID_HELP_WINDOWSVERSION,
                   L"Limited functionality, update to Windows 10 May 2020 Update (2004)!");
    }

    if(CanDisableBorder())
    {
        CheckMenuItem(GetSubMenu(m_mainMenu, 1), IDM_INPUT_REMOVEBORDER, MF_CHECKED | MF_BYCOMMAND);

        ModifyMenu(GetSubMenu(m_mainMenu, 4),
                   ID_HELP_WINDOWSVERSION,
                   MF_BYCOMMAND | MF_STRING | MF_DISABLED,
                   ID_HELP_WINDOWSVERSION,
                   L"Excellent functionality, Windows 11");
    }

    SetMenu(m_mainWindow, m_mainMenu);
    srand(static_cast<unsigned>(time(NULL)));
    RegisterHotKey(m_mainWindow, HK_FULLSCREEN, MOD_CONTROL | MOD_SHIFT, 0x47); // G
    RegisterHotKey(m_mainWindow, HK_SCREENSHOT, MOD_CONTROL | MOD_SHIFT, 0x53); // S
    RegisterHotKey(m_mainWindow, HK_PAUSE, MOD_CONTROL | MOD_SHIFT, 0x50); // P

    m_captureOptions.monitor      = nullptr;
    m_captureOptions.outputWindow = m_mainWindow;

    // set defaults
    SendMessage(m_mainWindow, WM_COMMAND, WM_PIXEL_SIZE(2), 0);
    SendMessage(m_mainWindow, WM_COMMAND, WM_ASPECT_RATIO(0), 0);
    SendMessage(m_mainWindow, WM_COMMAND, WM_SHADER(39), 0);
    SendMessage(m_mainWindow, WM_COMMAND, WM_FRAME_SKIP(1), 0);
    SendMessage(m_mainWindow, WM_COMMAND, WM_OUTPUT_SCALE(0), 0);
    SendMessage(m_mainWindow, WM_COMMAND, WM_CAPTURE_DISPLAY(0), 0);
    SendMessage(m_mainWindow, WM_COMMAND, Is1903() ? IDM_MODE_CLONE : IDM_MODE_GLASS, 0);

    return TRUE;
}

void ShaderWindow::Start(_In_ LPWSTR lpCmdLine, HWND paramsWindow)
{
    bool autoStart  = true;
    bool fullScreen = false;

    if(lpCmdLine)
    {
        int  numArgs;
        auto cmdLine = GetCommandLineW();
        auto args    = CommandLineToArgvW(cmdLine, &numArgs);

        for(int a = 0; a < numArgs; a++)
        {
            if(wcscmp(args[a], L"-paused") == 0 || wcscmp(args[a], L"-p") == 0)
                autoStart = false;
            else if(wcscmp(args[a], L"-fullscreen") == 0 || wcscmp(args[a], L"-f") == 0)
                fullScreen = true;
            else if(a == numArgs - 1)
            {
                std::wstring ws(args[a]);
                if(ws.size())
                    LoadProfile(std::string(ws.begin(), ws.end()));
            }
        }
    }

    m_paramsWindow = paramsWindow;
    m_inputDialog.reset(new InputDialog(m_instance, m_mainWindow));

    if(autoStart)
    {
        SendMessage(m_mainWindow, WM_COMMAND, IDM_START, 0);
        SendMessage(m_paramsWindow, WM_COMMAND, IDM_UPDATE_PARAMS, 0);
    }
    if(fullScreen)
    {
        SendMessage(m_mainWindow, WM_COMMAND, ID_PROCESSING_FULLSCREEN, 0);
    }
}