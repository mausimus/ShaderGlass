#include "pch.h"

#include "resource.h"
#include "ShaderWindow.h"
#include "ShaderGC.h"

ShaderWindow::ShaderWindow(CaptureManager& captureManager) :
    m_captureManager(captureManager), m_captureOptions(captureManager.m_options), m_title(), m_windowClass(), m_toggledNone(false)
{ }

bool ShaderWindow::LoadProfile(const std::wstring& fileName)
{
    try
    {
        bool paused = m_captureManager.IsActive();
        if(paused)
            Stop();

        m_captureManager.ForgetLastPreset();

        std::ifstream infile(fileName);
        if(!infile.good())
        {
            auto message = std::wstring(TEXT("Unable to find profile ")) + fileName;
            MessageBox(NULL, message.data(), L"ShaderGlass", MB_OK | MB_ICONERROR);

            return false;
        }

        std::string                                       shaderCategory;
        std::string                                       shaderName;
        std::optional<std::wstring>                       windowName;
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
                    return true;
            }
            else if(key == "CaptureWindow")
            {
                wchar_t wideName[MAX_WINDOW_TITLE];
                MultiByteToWideChar(CP_UTF8, 0, value.c_str(), -1, wideName, MAX_WINDOW_TITLE);
                windowName = std::wstring(wideName);
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
            else if(key == "DPIScaling")
            {
                if(value == "1")
                {
                    m_captureOptions.dpiScale = m_dpiScale;
                    CheckMenuItem(m_pixelSizeMenu, IDM_PIXELSIZE_DPI, MF_CHECKED | MF_BYCOMMAND);
                }
                else
                {
                    m_captureOptions.dpiScale = 1.0f;
                    CheckMenuItem(m_pixelSizeMenu, IDM_PIXELSIZE_DPI, MF_UNCHECKED | MF_BYCOMMAND);
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
                if(value == "Free")
                {
                    CheckMenuRadioItem(m_outputScaleMenu, WM_OUTPUT_SCALE(0), WM_OUTPUT_SCALE(static_cast<UINT>(outputScales.size() - 1)), 0, MF_BYCOMMAND);
                    CheckMenuItem(m_outputScaleMenu, IDM_OUTPUT_FREESCALE, MF_CHECKED | MF_BYCOMMAND);
                    m_captureOptions.freeScale   = true;
                    m_captureOptions.outputScale = 1.0f;
                }
                else
                {
                    CheckMenuItem(m_outputScaleMenu, IDM_OUTPUT_FREESCALE, MF_UNCHECKED | MF_BYCOMMAND);
                    m_captureOptions.freeScale = false;
                    for(const auto& p : outputScales)
                    {
                        if(value == p.second.mnemonic)
                            SendMessage(m_mainWindow, WM_COMMAND, p.first, 0);
                    }
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
            else if(key == "InputArea")
            {
                std::istringstream iss(value);
                iss >> m_captureOptions.inputArea.left;
                iss >> m_captureOptions.inputArea.top;
                iss >> m_captureOptions.inputArea.right;
                iss >> m_captureOptions.inputArea.bottom;
                if(m_captureOptions.inputArea.right - m_captureOptions.inputArea.left != 0)
                    CheckMenuItem(m_displayMenu, ID_DESKTOP_LOCKINPUTAREA, MF_CHECKED | MF_BYCOMMAND);
                else
                    CheckMenuItem(m_displayMenu, ID_DESKTOP_LOCKINPUTAREA, MF_UNCHECKED | MF_BYCOMMAND);
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

        AddRecentProfile(fileName);
    }
    catch(std::exception& e)
    {
        MessageBox(NULL, convertCharArrayToLPCWSTR((std::string("Error loading profile: ") + std::string(e.what())).c_str()), L"ShaderGlass", MB_OK | MB_ICONERROR);
    }

    return true;
}

void ShaderWindow::LoadProfile()
{
    OPENFILENAMEW ofn;
    wchar_t       szFileName[MAX_PATH] = L"";
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
        LoadProfile(ws);
    }
}

void ShaderWindow::ImportShader()
{
    OPENFILENAMEW ofn;
    wchar_t       szFileName[MAX_PATH] = L"";
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner   = NULL;
    ofn.lpstrFilter =
        (LPCWSTR)L"Slang Profiles or Shaders (*.slangp;*.slang)\0*.slangp;*.slang\0Slang Profiles (*.slangp)\0*.slangp\0Slang Shaders (*.slang)\0*.slang\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile   = (LPWSTR)szFileName;
    ofn.nMaxFile    = MAX_PATH;
    ofn.Flags       = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = (LPCWSTR)L"slangp";

    if(GetOpenFileName(&ofn))
    {
        std::wstring ws(ofn.lpstrFile);
        ImportShader(ws);
    }
}

void ShaderWindow::SetFreeScale()
{
    CheckMenuRadioItem(m_outputScaleMenu, WM_OUTPUT_SCALE(0), WM_OUTPUT_SCALE(static_cast<UINT>(outputScales.size() - 1)), 0, MF_BYCOMMAND);
    CheckMenuItem(m_outputScaleMenu, IDM_OUTPUT_FREESCALE, MF_CHECKED | MF_BYCOMMAND);
    m_captureOptions.freeScale = true;
    m_captureManager.UpdateOutputSize();
}

void ShaderWindow::LoadImage()
{
    OPENFILENAMEW ofn;
    wchar_t       szFileName[MAX_PATH] = L"";
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner   = NULL;
    ofn.lpstrFilter = (LPCWSTR)L"Images (*.png;*.jpg)\0*.png;*.jpg\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile   = (LPWSTR)szFileName;
    ofn.nMaxFile    = MAX_PATH;
    ofn.Flags       = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = (LPCWSTR)L"png";

    if(GetOpenFileName(&ofn))
    {
        std::wstring ws(ofn.lpstrFile);
        m_captureOptions.imageFile     = ws;
        m_captureOptions.captureWindow = NULL;
        m_captureOptions.monitor       = NULL;

        // update input checkboxes
        CheckMenuRadioItem(m_windowMenu, WM_CAPTURE_WINDOW(0), WM_CAPTURE_WINDOW(static_cast<UINT>(m_captureWindows.size())), 0, MF_BYCOMMAND);
        CheckMenuRadioItem(m_displayMenu, WM_CAPTURE_DISPLAY(0), WM_CAPTURE_DISPLAY(static_cast<UINT>(m_captureDisplays.size())), 0, MF_BYCOMMAND);
        auto prevState   = CheckMenuItem(m_inputMenu, ID_INPUT_FILE, MF_CHECKED | MF_BYCOMMAND);
        auto setDefaults = prevState != MF_CHECKED;

        // default to solid clone
        m_captureOptions.clone = true;
        CheckMenuItem(m_modeMenu, IDM_MODE_GLASS, MF_UNCHECKED | MF_BYCOMMAND);
        CheckMenuItem(m_modeMenu, IDM_MODE_CLONE, MF_CHECKED | MF_BYCOMMAND);
        m_captureOptions.transparent = false;
        CheckMenuItem(m_outputWindowMenu, IDM_WINDOW_TRANSPARENT, MF_UNCHECKED | MF_BYCOMMAND);
        CheckMenuItem(m_outputWindowMenu, IDM_WINDOW_SOLID, MF_CHECKED | MF_BYCOMMAND);
        TryUpdateInput();
        EnableMenuItem(m_outputScaleMenu, IDM_OUTPUT_FREESCALE, MF_BYCOMMAND | MF_ENABLED);

        // if we are *switching* to file mode, default pixel size and freescale
        if(setDefaults)
        {
            // set starting scale to fit within current window size
            RECT r;
            GetClientRect(m_mainWindow, &r);
            int defaultScale = 1;
            if(m_captureOptions.imageWidth > 0 && m_captureOptions.imageHeight > 0)
            {
                defaultScale = max(1, min(r.right / m_captureOptions.imageWidth, r.bottom / m_captureOptions.imageHeight));
            }

            m_captureOptions.outputScale = defaultScale;
            SendMessage(m_mainWindow, WM_COMMAND, WM_PIXEL_SIZE(0), 0);
            SetFreeScale();
        }

        UpdateWindowState();
    }
}

void ShaderWindow::SaveProfile(const std::wstring& fileName)
{
    const auto& pixelSize   = pixelSizes.at(WM_PIXEL_SIZE(m_selectedPixelSize));
    const auto& outputScale = outputScales.at(WM_OUTPUT_SCALE(m_selectedOutputScale));
    const auto& aspectRatio = aspectRatios.at(WM_ASPECT_RATIO(m_selectedAspectRatio));
    const auto& frameSkip   = frameSkips.at(WM_FRAME_SKIP(m_selectedFrameSkip));
    const auto& shader      = m_captureManager.Presets().at(m_captureOptions.presetNo);

    std::ofstream outfile(fileName);
    outfile << "ProfileVersion " << std::quoted("1.0") << std::endl;
    outfile << "PixelSize " << std::quoted(pixelSize.mnemonic) << std::endl;
    outfile << "DPIScaling " << std::quoted(std::to_string(m_captureOptions.dpiScale != 1.0f)) << std::endl;
    if(aspectRatio.mnemonic == CUSTOM_MNEMONIC)
        outfile << "AspectRatio " << std::quoted(std::to_string(aspectRatio.r)) << std::endl;
    else
        outfile << "AspectRatio " << std::quoted(aspectRatio.mnemonic) << std::endl;
    outfile << "ShaderCategory " << std::quoted(shader->Category) << std::endl;
    outfile << "ShaderName " << std::quoted(shader->Name) << std::endl;
    outfile << "FrameSkip " << std::quoted(frameSkip.mnemonic) << std::endl;
    outfile << "OutputScale " << std::quoted(m_captureOptions.freeScale ? "Free" : outputScale.mnemonic) << std::endl;
    outfile << "FlipH " << std::quoted(std::to_string(m_captureOptions.flipHorizontal)) << std::endl;
    outfile << "FlipV " << std::quoted(std::to_string(m_captureOptions.flipVertical)) << std::endl;
    outfile << "Clone " << std::quoted(std::to_string(m_captureOptions.clone)) << std::endl;
    outfile << "CaptureCursor " << std::quoted(std::to_string(m_captureOptions.captureCursor)) << std::endl;
    outfile << "Transparent " << std::quoted(std::to_string(m_captureOptions.transparent)) << std::endl;
    outfile << "InputArea \"" << std::to_string(m_captureOptions.inputArea.left) << " " << std::to_string(m_captureOptions.inputArea.top) << " "
            << std::to_string(m_captureOptions.inputArea.right) << " " << std::to_string(m_captureOptions.inputArea.bottom) << "\"" << std::endl;
    if(m_captureOptions.captureWindow)
    {
        auto windowTitle = GetWindowStringText(m_captureOptions.captureWindow);
        char utfName[MAX_WINDOW_TITLE];
        WideCharToMultiByte(CP_UTF8, 0, windowTitle.c_str(), -1, utfName, MAX_WINDOW_TITLE, NULL, NULL);
        outfile << "CaptureWindow " << std::quoted(utfName) << std::endl;
    }
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

    AddRecentProfile(fileName);
}

void ShaderWindow::SaveProfile()
{
    OPENFILENAMEW ofn;
    wchar_t       szFileName[MAX_PATH] = L"";
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
        SaveProfile(ws);
    }
}

DWORD WINAPI CompileThreadFuncProxy(LPVOID lpParam)
{
    ((ShaderWindow*)lpParam)->CompileThreadFunc();
    return 0;
}

// shader compiler needs to reuse thread
void ShaderWindow::CompileThreadFunc()
{
    while(true)
    {
        WaitForSingleObject(m_compileEvent, INFINITE);
        ResetEvent(m_compileEvent);

        if(m_importPath.empty())
            continue;

        std::string errorMsg;
        try
        {
            std::ofstream log;
            bool          warn;
            auto          preset = ShaderGC::CompilePreset(m_importPath, log, warn);
            if(preset == nullptr)
                throw std::runtime_error("Internal error");
            auto          id     = m_captureManager.AddPreset(preset);
            m_numPresets         = m_captureManager.Presets().size();
            SendMessage(m_browserWindow, WM_COMMAND, WM_USER + 1, id);
            SendMessage(m_mainWindow, WM_COMMAND, WM_SHADER(id), 0);
        }
        catch(std::exception& ex)
        {
            errorMsg = std::string(ex.what());
        }
        EnableWindow(m_mainWindow, true);
        ShowWindow(m_compileWindow, SW_HIDE);
        m_importPath = std::filesystem::path();

        if(errorMsg.size())
        {
            MessageBox(m_mainWindow, convertCharArrayToLPCWSTR(errorMsg.c_str()), L"ShaderGlass", MB_OK);
        }
    }
}

bool ShaderWindow::ImportShader(const std::wstring& fileName)
{
    try
    {
        m_importPath = fileName;

        RECT rc, rcDlg, rcOwner;
        GetWindowRect(m_mainWindow, &rcOwner);
        GetWindowRect(m_compileWindow, &rcDlg);
        CopyRect(&rc, &rcOwner);
        OffsetRect(&rcDlg, -rcDlg.left, -rcDlg.top);
        OffsetRect(&rc, -rc.left, -rc.top);
        OffsetRect(&rc, -rcDlg.right, -rcDlg.bottom);
        SetWindowPos(m_compileWindow,
                     HWND_TOP,
                     rcOwner.left + (rc.right / 2),
                     rcOwner.top + max(0, (rc.bottom / 2)),
                     0,
                     0, // Ignores size arguments.
                     SWP_NOSIZE);

        ShowWindow(m_compileWindow, SW_SHOW);
        EnableWindow(m_mainWindow, false);
        if(!m_compileEvent)
        {
            m_compileEvent = CreateEvent(NULL, TRUE, FALSE, TEXT("CompileEvent"));
        }
        if(!m_compileThread)
        {
            m_compileThread = CreateThread(NULL, 0, CompileThreadFuncProxy, this, 0, NULL);
        }
        if(m_compileEvent)
        {
            SetEvent(m_compileEvent);
        }
        return true;
    }
    catch(std::exception& ex)
    {
        MessageBox(m_mainWindow, convertCharArrayToLPCWSTR(ex.what()), L"ShaderGlass", MB_OK);
        return false;
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
        AppendMenu(m_windowMenu, MF_STRING, WM_CAPTURE_WINDOW(i++), w.name.c_str());
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
        InsertMenu(m_displayMenu, 2, MF_STRING, WM_CAPTURE_DISPLAY(i++), convertCharArrayToLPCWSTR(w.name.c_str()));
        if(m_captureOptions.monitor == w.monitor)
            CheckMenuItem(m_displayMenu, WM_CAPTURE_DISPLAY(i - 1), MF_CHECKED | MF_BYCOMMAND);
    }
}

void ShaderWindow::BuildProgramMenu()
{
    m_recentMenu = CreatePopupMenu();
    InsertMenu(m_programMenu, 9, MF_BYPOSITION | MF_STRING | MF_POPUP, (UINT_PTR)m_recentMenu, L"Recent profiles");
    LoadRecentProfiles();
}

void ShaderWindow::BuildInputMenu()
{
    m_inputMenu = GetSubMenu(m_mainMenu, 1);

    RemoveMenu(GetSubMenu(m_inputMenu, 0), ID_DESKTOP_DUMMY, MF_BYCOMMAND);

    m_pixelSizeMenu = CreatePopupMenu();
    AppendMenu(m_pixelSizeMenu, MF_STRING, IDM_PIXELSIZE_NEXT, L"Next\tp");

    auto systemDpi = GetDpiForSystem();
    char dpiMenu[100];
    m_dpiScale = GetDpiForSystem() / 96.0f;
    snprintf(dpiMenu, 100, "Adjust for DPI Scale (%d%%)", static_cast<int>(100.0f * m_dpiScale));
    AppendMenu(m_pixelSizeMenu, MF_STRING, IDM_PIXELSIZE_DPI, convertCharArrayToLPCWSTR(dpiMenu));
    if(systemDpi == 96)
    {
        // no scaling can be applied
        m_dpiScale = 1.0f;
        EnableMenuItem(m_pixelSizeMenu, IDM_PIXELSIZE_DPI, MF_BYCOMMAND | MF_DISABLED);
    }
    for(const auto& px : pixelSizes)
    {
        AppendMenu(m_pixelSizeMenu, MF_STRING, px.first, convertCharArrayToLPCWSTR(px.second.text));
    }
    InsertMenu(m_inputMenu, 4, MF_BYPOSITION | MF_STRING | MF_POPUP, (UINT_PTR)m_pixelSizeMenu, L"Pixel Size");

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
    AppendMenu(m_outputScaleMenu, MF_STRING, IDM_OUTPUT_FREESCALE, L"Free");
    for(const auto& os : outputScales)
    {
        AppendMenu(m_outputScaleMenu, MF_STRING, os.first, os.second.text);
    }
    InsertMenu(sMenu, 3, MF_BYPOSITION | MF_STRING | MF_POPUP, (UINT_PTR)m_outputScaleMenu, L"Scale");

    m_aspectRatioMenu = CreatePopupMenu();
    for(const auto& ar : aspectRatios)
    {
        AppendMenu(m_aspectRatioMenu, MF_STRING, ar.first, ar.second.text);
    }
    InsertMenu(sMenu, 4, MF_BYPOSITION | MF_STRING | MF_POPUP, (UINT_PTR)m_aspectRatioMenu, L"Aspect Ratio Correction");

    m_frameSkipMenu = CreatePopupMenu();
    for(const auto& fs : frameSkips)
    {
        AppendMenu(m_frameSkipMenu, MF_STRING, fs.first, fs.second.text);
    }
    InsertMenu(sMenu, 5, MF_BYPOSITION | MF_STRING | MF_POPUP, (UINT_PTR)m_frameSkipMenu, L"FPS");

    InsertMenu(sMenu, 6, MF_BYPOSITION | MF_STRING, ID_PROCESSING_FULLSCREEN, L"Fullscreen\tCtrl+Shift+G");
}

void ShaderWindow::BuildShaderMenu()
{
    // now deferred to BrowserWindow
    m_numPresets = m_captureManager.Presets().size();
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
    if(m_isBorderless)
        return;

    // resize client area to captured window/file x scale
    if(m_captureManager.IsActive() && !m_captureOptions.freeScale && ((m_captureOptions.captureWindow && m_captureOptions.clone) || !m_captureOptions.imageFile.empty()))
    {
        LONG inputWidth = 0, inputHeight = 0;

        if(m_captureOptions.captureWindow != 0)
        {
            RECT captureRect;
            GetClientRect(m_captureOptions.captureWindow, &captureRect);
            inputWidth  = captureRect.right;
            inputHeight = captureRect.bottom;
        }
        else
        {
            inputWidth  = m_captureOptions.imageWidth;
            inputHeight = m_captureOptions.imageHeight;
        }

        RECT r;
        GetClientRect(hWnd, &r);
        auto requiredW = static_cast<LONG>(inputWidth * m_captureOptions.outputScale / m_captureOptions.aspectRatio);
        auto requiredH = static_cast<LONG>(inputHeight * m_captureOptions.outputScale);
        if(requiredW == 0)
            requiredW = 1;
        if(requiredH == 0)
            requiredH = 1;

        if(r.right != requiredW || r.bottom != requiredH)
        {
            r.right  = requiredW;
            r.bottom = requiredH;
            AdjustWindowRect(&r, GetWindowLong(hWnd, GWL_STYLE), TRUE);
            SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, r.right - r.left, r.bottom - r.top, SWP_NOMOVE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOACTIVATE);
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
                             SWP_NOMOVE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOACTIVATE);
            }
        }
    }
}

void ShaderWindow::UpdateWindowState()
{
    // always topmost when processing
    if(m_captureManager.IsActive())
        SetWindowPos(m_mainWindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    else
        SetWindowPos(m_mainWindow, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

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

    UpdateTitle();

    AdjustWindowSize(m_mainWindow);
}

void ShaderWindow::UpdateTitle()
{
    // update title
    if(m_captureManager.IsActive())
    {
        const auto& pixelSize   = pixelSizes.at(WM_PIXEL_SIZE(m_selectedPixelSize));
        const auto& outputScale = outputScales.at(WM_OUTPUT_SCALE(m_selectedOutputScale));
        const auto& aspectRatio = aspectRatios.at(WM_ASPECT_RATIO(m_selectedAspectRatio));
        const auto& shader      = m_captureManager.Presets().at(m_captureOptions.presetNo);

        wchar_t windowName[26];
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
                    captureTitle = captureTitle.substr(0, 20) + _T("...");
                }
                captureTitle += _T(", ");
                wcsncpy_s(windowName, captureTitle.c_str(), 26);
            }
        }

        wchar_t     title[200];
        const char* scaleString = m_captureOptions.freeScale ? "free" : outputScale.mnemonic;
        const auto  fps         = (int)roundf(m_captureManager.FPS());
        _snwprintf_s(title, 200, _T("ShaderGlass (%s%S, %Spx, %S%%, ~%S, %dfps)"), windowName, shader->Name.c_str(), pixelSize.mnemonic, scaleString, aspectRatio.mnemonic, fps);
        SetWindowTextW(m_mainWindow, title);
    }
    else
    {
        SetWindowTextW(m_mainWindow, _T("ShaderGlass (stopped)"));
    }
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
        case ID_SHADER_BROWSE: {
            if(!m_browserPositioned)
            {
                RECT rc, rcDlg, rcOwner;
                GetWindowRect(m_mainWindow, &rcOwner);
                GetWindowRect(m_browserWindow, &rcDlg);
                CopyRect(&rc, &rcOwner);
                OffsetRect(&rcDlg, -rcDlg.left, -rcDlg.top);
                OffsetRect(&rc, -rc.left, -rc.top);
                OffsetRect(&rc, -rcDlg.right, -rcDlg.bottom);

                if(!SetWindowPos(m_browserWindow,
                                 HWND_TOP,
                                 rcOwner.right - (rcDlg.right - rcDlg.left),
                                 rcOwner.top + max(0, (rc.bottom / 2)),
                                 0,
                                 0, // Ignores size arguments.
                                 SWP_NOSIZE))
                {
                    int x = 4;
                    x++;
                }
                m_browserPositioned = true;
            }
            ShowWindow(m_browserWindow, SW_SHOW);
        }
            return 0;
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
        case ID_INPUT_FILE:
            LoadImage();
            break;
        case ID_PROCESSING_GLOBALHOTKEYS:
            if(GetMenuState(m_programMenu, ID_PROCESSING_GLOBALHOTKEYS, MF_BYCOMMAND) & MF_CHECKED)
            {
                UnregisterHotkeys();
                CheckMenuItem(m_programMenu, ID_PROCESSING_GLOBALHOTKEYS, MF_UNCHECKED);
                SaveHotkeyState(false);
            }
            else
            {
                RegisterHotkeys();
                CheckMenuItem(m_programMenu, ID_PROCESSING_GLOBALHOTKEYS, MF_CHECKED);
                SaveHotkeyState(true);
            }
            break;
        case ID_DESKTOP_LOCKINPUTAREA:
            if(m_captureOptions.inputArea.right - m_captureOptions.inputArea.left != 0)
            {
                m_captureOptions.inputArea.top    = 0;
                m_captureOptions.inputArea.left   = 0;
                m_captureOptions.inputArea.bottom = 0;
                m_captureOptions.inputArea.right  = 0;
                CheckMenuItem(m_displayMenu, ID_DESKTOP_LOCKINPUTAREA, MF_UNCHECKED | MF_BYCOMMAND);
            }
            else
            {
                POINT topLeft;
                topLeft.x = 0;
                topLeft.y = 0;
                ClientToScreen(hWnd, &topLeft);
                RECT clientArea;
                GetClientRect(hWnd, &clientArea);
                m_captureOptions.inputArea.top    = topLeft.y;
                m_captureOptions.inputArea.left   = topLeft.x;
                m_captureOptions.inputArea.bottom = topLeft.y + clientArea.bottom;
                m_captureOptions.inputArea.right  = topLeft.x + clientArea.right;
                CheckMenuItem(m_displayMenu, ID_DESKTOP_LOCKINPUTAREA, MF_CHECKED | MF_BYCOMMAND);
            }
            m_captureManager.UpdateLockedArea();
            break;
        case IDM_PIXELSIZE_DPI:
            if(m_captureOptions.dpiScale == m_dpiScale)
            {
                m_captureOptions.dpiScale = 1.0f; // disable DPI scaling
                CheckMenuItem(m_pixelSizeMenu, IDM_PIXELSIZE_DPI, MF_UNCHECKED | MF_BYCOMMAND);
            }
            else
            {
                m_captureOptions.dpiScale = m_dpiScale; // enable DPI scaling
                CheckMenuItem(m_pixelSizeMenu, IDM_PIXELSIZE_DPI, MF_CHECKED | MF_BYCOMMAND);
            }
            m_captureManager.UpdatePixelSize();
            break;
        case IDM_OUTPUT_FREESCALE:
            if(!m_captureOptions.freeScale)
            {
                CheckMenuRadioItem(m_outputScaleMenu, WM_OUTPUT_SCALE(0), WM_OUTPUT_SCALE(static_cast<UINT>(outputScales.size() - 1)), 0, MF_BYCOMMAND);
                CheckMenuItem(m_outputScaleMenu, IDM_OUTPUT_FREESCALE, MF_CHECKED | MF_BYCOMMAND);
                m_captureOptions.freeScale   = true;
                m_captureOptions.outputScale = 1.0f;
            }
            m_captureManager.UpdateOutputSize();
            UpdateWindowState();
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
        case IDM_FULLSCREEN:
            ToggleBorderless(hWnd);
            break;
        case IDM_SCREENSHOT:
            Screenshot();
            break;
        case IDM_PAUSE:
            if(m_captureManager.IsActive())
                Stop();
            else
                Start();
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
            TryUpdateInput();
            UpdateWindowState();
            break;
        case IDM_MODE_CLONE:
            m_captureOptions.clone = true;
            CheckMenuItem(m_modeMenu, IDM_MODE_GLASS, MF_UNCHECKED | MF_BYCOMMAND);
            CheckMenuItem(m_modeMenu, IDM_MODE_CLONE, MF_CHECKED | MF_BYCOMMAND);
            m_captureOptions.transparent = false;
            CheckMenuItem(m_outputWindowMenu, IDM_WINDOW_TRANSPARENT, MF_UNCHECKED | MF_BYCOMMAND);
            CheckMenuItem(m_outputWindowMenu, IDM_WINDOW_SOLID, MF_CHECKED | MF_BYCOMMAND);
            TryUpdateInput();
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
        case ID_SHADER_IMPORT:
            ImportShader();
            break;
        case IDM_PROCESSING_SAVEPROFILEAS:
            SaveProfile();
            break;
        case IDM_EXIT:
            m_captureManager.StopSession();
            DestroyWindow(hWnd);
            break;
        case ID_QUICK_TOGGLE: {
            bool isChecked = GetMenuState(m_shaderMenu, ID_QUICK_TOGGLE, MF_BYCOMMAND) & MF_CHECKED;
            if(lParam == 1 || (lParam == 0 && isChecked)) // down/disable
            {
                if(!m_toggledNone)
                {
                    m_toggledNone     = true;
                    m_toggledPresetNo = m_captureOptions.presetNo;
                    m_captureManager.RememberLastPreset();
                    SendMessage(hWnd, WM_COMMAND, WM_SHADER(0), 1);
                    CheckMenuItem(m_shaderMenu, ID_QUICK_TOGGLE, MF_UNCHECKED | MF_BYCOMMAND);
                }
            }
            else if(lParam == 2 || (lParam == 0 && !isChecked)) // up/enable
            {
                if(m_toggledNone)
                {
                    m_toggledNone = false;
                    m_captureManager.SetLastPreset(m_toggledPresetNo);
                    SendMessage(hWnd, WM_COMMAND, WM_SHADER(m_toggledPresetNo), 1);
                    CheckMenuItem(m_shaderMenu, ID_QUICK_TOGGLE, MF_CHECKED | MF_BYCOMMAND);
                }
            }
        }
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
        case ID_HELP_README:
            ShellExecute(0, 0, L"https://github.com/mausimus/ShaderGlass/blob/master/README.md", 0, 0, SW_SHOW);
            break;
        case ID_HELP_FREQUENTLYASKEDQUESTIONS:
            ShellExecute(0, 0, L"https://github.com/mausimus/ShaderGlass/blob/master/FAQ.md", 0, 0, SW_SHOW);
            break;
        default:
            if(wmId >= WM_USER && wmId <= 0x7FFF)
            {
                if(wmId >= WM_SHADER(0) && wmId < WM_SHADER(MAX_SHADERS))
                {
                    PostMessage(m_browserWindow, WM_COMMAND, WM_USER, wmId + (lParam << 16));
                    m_captureOptions.presetNo = wmId - WM_SHADER(0);
                    m_captureManager.UpdateShaderPreset();
                    UpdateWindowState();
                    if(wmId != WM_SHADER(0) && m_toggledNone)
                    {
                        m_toggledNone = false;
                        CheckMenuItem(m_shaderMenu, ID_QUICK_TOGGLE, MF_CHECKED | MF_BYCOMMAND);
                    }
                    break;
                }
                if(wmId >= WM_CAPTURE_WINDOW(0) && wmId < WM_CAPTURE_WINDOW(MAX_CAPTURE_WINDOWS))
                {
                    CheckMenuRadioItem(m_windowMenu, WM_CAPTURE_WINDOW(0), WM_CAPTURE_WINDOW(static_cast<UINT>(m_captureWindows.size())), wmId, MF_BYCOMMAND);
                    CheckMenuRadioItem(m_displayMenu, WM_CAPTURE_DISPLAY(0), WM_CAPTURE_DISPLAY(static_cast<UINT>(m_captureDisplays.size())), 0, MF_BYCOMMAND);
                    m_captureOptions.captureWindow = m_captureWindows.at(wmId - WM_CAPTURE_WINDOW(0)).hwnd;
                    m_captureOptions.monitor       = NULL;
                    m_captureOptions.clone         = true;
                    m_captureOptions.transparent   = false;
                    CheckMenuItem(m_modeMenu, IDM_MODE_GLASS, MF_UNCHECKED | MF_BYCOMMAND);
                    CheckMenuItem(m_modeMenu, IDM_MODE_CLONE, MF_CHECKED | MF_BYCOMMAND);
                    CheckMenuItem(m_outputWindowMenu, IDM_WINDOW_TRANSPARENT, MF_UNCHECKED | MF_BYCOMMAND);
                    CheckMenuItem(m_outputWindowMenu, IDM_WINDOW_SOLID, MF_CHECKED | MF_BYCOMMAND);
                    CheckMenuItem(m_inputMenu, ID_INPUT_FILE, MF_UNCHECKED | MF_BYCOMMAND);
                    EnableMenuItem(m_outputScaleMenu, IDM_OUTPUT_FREESCALE, MF_BYCOMMAND | MF_ENABLED);
                    m_captureOptions.imageFile.clear();
                    TryUpdateInput();
                    UpdateWindowState();
                    SetFreeScale();
                    break;
                }
                if(wmId >= WM_CAPTURE_DISPLAY(0) && wmId < WM_CAPTURE_DISPLAY(MAX_CAPTURE_DISPLAYS))
                {
                    CheckMenuRadioItem(m_windowMenu, WM_CAPTURE_WINDOW(0), WM_CAPTURE_WINDOW(static_cast<UINT>(m_captureWindows.size())), 0, MF_BYCOMMAND);
                    CheckMenuRadioItem(m_displayMenu, WM_CAPTURE_DISPLAY(0), WM_CAPTURE_DISPLAY(static_cast<UINT>(m_captureDisplays.size())), wmId, MF_BYCOMMAND);
                    m_captureOptions.captureWindow = NULL;
                    m_captureOptions.monitor       = m_captureDisplays.at(wmId - WM_CAPTURE_DISPLAY(0)).monitor;
                    m_captureOptions.clone         = false;
                    m_captureOptions.transparent   = true;
                    if(m_captureOptions.freeScale)
                    {
                        CheckMenuItem(m_outputScaleMenu, IDM_OUTPUT_FREESCALE, MF_UNCHECKED | MF_BYCOMMAND);
                        m_captureOptions.freeScale   = false;
                        m_captureOptions.outputScale = 1.0f;
                        for(const auto& p : outputScales)
                        {
                            if(m_captureOptions.outputScale == p.second.s)
                                CheckMenuItem(m_outputScaleMenu, p.first, MF_CHECKED | MF_BYCOMMAND);
                        }
                    }
                    CheckMenuItem(m_modeMenu, IDM_MODE_GLASS, MF_CHECKED | MF_BYCOMMAND);
                    CheckMenuItem(m_modeMenu, IDM_MODE_CLONE, MF_UNCHECKED | MF_BYCOMMAND);
                    CheckMenuItem(m_outputWindowMenu, IDM_WINDOW_TRANSPARENT, MF_CHECKED | MF_BYCOMMAND);
                    CheckMenuItem(m_outputWindowMenu, IDM_WINDOW_SOLID, MF_UNCHECKED | MF_BYCOMMAND);
                    CheckMenuItem(m_inputMenu, ID_INPUT_FILE, MF_UNCHECKED | MF_BYCOMMAND);
                    EnableMenuItem(m_outputScaleMenu, IDM_OUTPUT_FREESCALE, MF_BYCOMMAND | MF_DISABLED);
                    m_captureOptions.imageFile.clear();
                    TryUpdateInput();
                    UpdateWindowState();
                    break;
                }
                const auto& pixelSize = pixelSizes.find(wmId);
                if(pixelSize != pixelSizes.end())
                {
                    m_selectedPixelSize = wmId - WM_PIXEL_SIZE(0);
                    CheckMenuRadioItem(m_pixelSizeMenu, WM_PIXEL_SIZE(0), WM_PIXEL_SIZE(static_cast<UINT>(pixelSizes.size() - 1)), wmId, MF_BYCOMMAND);
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
                    CheckMenuRadioItem(m_outputScaleMenu, WM_OUTPUT_SCALE(0), WM_OUTPUT_SCALE(static_cast<UINT>(outputScales.size() - 1)), wmId, MF_BYCOMMAND);
                    CheckMenuItem(m_outputScaleMenu, IDM_OUTPUT_FREESCALE, MF_UNCHECKED | MF_BYCOMMAND);
                    m_captureOptions.outputScale = outputScale->second.s;
                    m_captureOptions.freeScale   = false;
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
                    CheckMenuRadioItem(m_aspectRatioMenu, 0, static_cast<UINT>(aspectRatios.size()), wmId - WM_ASPECT_RATIO(0), MF_BYPOSITION);
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
                if(wmId >= WM_RECENT_PROFILE(0) && wmId < WM_RECENT_PROFILE(MAX_RECENT_PROFILES))
                {
                    auto profileId = wmId - WM_RECENT_PROFILE(0);
                    if(profileId < m_recentProfiles.size())
                    {
                        auto path = m_recentProfiles.at(profileId);
                        if(!LoadProfile(path))
                        {
                            RemoveRecentProfile(path);
                        }
                    }
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
    case WM_KEYDOWN:
        if(wParam == VK_TAB)
        {
            SendMessage(hWnd, WM_COMMAND, ID_QUICK_TOGGLE, 1);
        }
        break;
    case WM_KEYUP:
        if(wParam == VK_TAB)
        {
            SendMessage(hWnd, WM_COMMAND, ID_QUICK_TOGGLE, 2);
        }
        break;
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
                if(m_captureManager.StartSession())
                {
                    m_captureOptions.paused = false;
                }
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
        case 0:
            UpdateTitle();
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

    if(m_captureManager.StartSession())
    {
        EnableMenuItem(m_programMenu, IDM_START, MF_BYCOMMAND | MF_DISABLED);
        EnableMenuItem(m_programMenu, IDM_STOP, MF_BYCOMMAND | MF_ENABLED);
    }
    else
    {
        EnableMenuItem(m_programMenu, IDM_START, MF_BYCOMMAND | MF_ENABLED);
        EnableMenuItem(m_programMenu, IDM_STOP, MF_BYCOMMAND | MF_DISABLED);
    }
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

void ShaderWindow::TryUpdateInput()
{
    if(!m_captureManager.UpdateInput())
    {
        EnableMenuItem(m_programMenu, IDM_START, MF_BYCOMMAND | MF_ENABLED);
        EnableMenuItem(m_programMenu, IDM_STOP, MF_BYCOMMAND | MF_DISABLED);
    }
}

void ShaderWindow::Screenshot()
{
    m_captureManager.GrabOutput();

    OPENFILENAME ofn;
    wchar_t      szFile[MAX_PATH] = L"";

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize     = sizeof(ofn);
    ofn.hwndOwner       = m_mainWindow;
    ofn.lpstrFile       = szFile;
    ofn.nMaxFile        = MAX_PATH;
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
        SetWindowPos(hWnd, HWND_TOPMOST, info.rcMonitor.left, info.rcMonitor.top, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, 0);
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
    m_shaderMenu  = GetSubMenu(m_mainMenu, 3);
    BuildProgramMenu();
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

        ModifyMenu(GetSubMenu(m_mainMenu, 4), ID_HELP_WINDOWSVERSION, MF_BYCOMMAND | MF_STRING | MF_DISABLED, ID_HELP_WINDOWSVERSION, L"Excellent functionality, Windows 11");
    }

    SetMenu(m_mainWindow, m_mainMenu);
    srand(static_cast<unsigned>(time(NULL)));
    if(GetHotkeyState())
    {
        RegisterHotkeys();
    }
    else
    {
        CheckMenuItem(m_programMenu, ID_PROCESSING_GLOBALHOTKEYS, MF_BYCOMMAND | MF_UNCHECKED);
    }

    m_captureOptions.monitor      = nullptr;
    m_captureOptions.outputWindow = m_mainWindow;

    // set defaults
    SendMessage(m_mainWindow, WM_COMMAND, WM_PIXEL_SIZE(2), 0);
    SendMessage(m_mainWindow, WM_COMMAND, WM_ASPECT_RATIO(0), 0);
    auto defaultNo = m_captureManager.FindByName(defaultPreset);
    if(defaultNo != -1)
    {
        SendMessage(m_mainWindow, WM_COMMAND, WM_SHADER(defaultNo), 0);
    }
    SendMessage(m_mainWindow, WM_COMMAND, WM_FRAME_SKIP(1), 0);
    SendMessage(m_mainWindow, WM_COMMAND, WM_OUTPUT_SCALE(0), 0);
    SendMessage(m_mainWindow, WM_COMMAND, WM_CAPTURE_DISPLAY(0), 0);
    SendMessage(m_mainWindow, WM_COMMAND, Is1903() ? IDM_MODE_CLONE : IDM_MODE_GLASS, 0);

    return TRUE;
}

void ShaderWindow::SaveHotkeyState(bool state)
{
    HKEY  hkey;
    DWORD dwDisposition;
    if(RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("Software\\ShaderGlass"), 0, NULL, 0, KEY_WRITE | KEY_SET_VALUE, NULL, &hkey, &dwDisposition) == ERROR_SUCCESS)
    {
        DWORD size  = sizeof(DWORD);
        DWORD value = state ? 1 : 0;
        RegSetValueEx(hkey, TEXT("Global Hotkeys"), 0, REG_DWORD, (PBYTE)&value, size);
        RegCloseKey(hkey);
    }
}

bool ShaderWindow::GetHotkeyState()
{
    HKEY hKey;
    if(RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("Software\\ShaderGlass"), 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
    {
        DWORD value = 1;
        DWORD size  = sizeof(DWORD);
        RegGetValue(hKey, NULL, TEXT("Global Hotkeys"), RRF_RT_REG_DWORD, NULL, &value, &size);
        return value == 1;
    }

    return true;
}

void ShaderWindow::LoadRecentProfiles()
{
    m_recentProfiles.clear();

    HKEY hKey;
    if(RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("Software\\ShaderGlass\\Recent"), 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
    {
        for(int p = 0; p < MAX_RECENT_PROFILES; p++)
        {
            auto value = std::to_wstring(p);

            wchar_t path[MAX_PATH + 1];
            DWORD   size = MAX_PATH * sizeof(wchar_t);
            if(RegGetValue(hKey, NULL, value.data(), RRF_RT_REG_SZ, NULL, path, &size) == ERROR_SUCCESS)
            {
                if(lstrlen(path) > 0)
                    m_recentProfiles.push_back(path);
            }
        }
    }

    // update menu
    for(UINT i = 0; i < MAX_RECENT_PROFILES; i++)
    {
        RemoveMenu(m_recentMenu, WM_RECENT_PROFILE(i), MF_BYCOMMAND);
    }
    for(int p = 0; p < m_recentProfiles.size(); p++)
    {
        const auto& profile = m_recentProfiles.at(p);
        InsertMenu(m_recentMenu, p, MF_STRING, WM_RECENT_PROFILE(p), profile.data());
    }
}

void ShaderWindow::SaveRecentProfiles()
{
    HKEY  hkey;
    DWORD dwDisposition;
    if(RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("Software\\ShaderGlass\\Recent"), 0, NULL, 0, KEY_WRITE | KEY_SET_VALUE, NULL, &hkey, &dwDisposition) == ERROR_SUCCESS)
    {
        for(int p = 0; p < MAX_RECENT_PROFILES; p++)
        {
            auto value = std::to_wstring(p);
            if(p < m_recentProfiles.size())
            {
                // update value
                const auto& path = m_recentProfiles.at(p);
                RegSetValueEx(hkey, value.data(), 0, REG_SZ, (PBYTE)path.data(), path.size() * sizeof(wchar_t));
            }
            else
            {
                // blank
                RegSetValueEx(hkey, value.data(), 0, REG_SZ, (PBYTE)TEXT(""), sizeof(wchar_t));
            }
        }
        RegCloseKey(hkey);
    }

    LoadRecentProfiles(); // rebuild menu
}

void ShaderWindow::AddRecentProfile(const std::wstring& path)
{
    if(path.find(L":") == std::wstring::npos) // don't store relative paths
        return;

    auto existingPos = std::find(m_recentProfiles.begin(), m_recentProfiles.end(), path);
    if(existingPos != m_recentProfiles.end())
    {
        // already first one
        if(*m_recentProfiles.begin() == path)
            return;

        // remove from the middle
        m_recentProfiles.erase(existingPos);
    }
    // add to front
    m_recentProfiles.insert(m_recentProfiles.begin(), path);
    if(m_recentProfiles.size() > MAX_RECENT_PROFILES)
    {
        m_recentProfiles.resize(MAX_RECENT_PROFILES);
    }
    SaveRecentProfiles();
}

void ShaderWindow::RemoveRecentProfile(const std::wstring& path)
{
    auto existingPos = std::find(m_recentProfiles.begin(), m_recentProfiles.end(), path);
    if(existingPos != m_recentProfiles.end())
    {
        m_recentProfiles.erase(existingPos);
        SaveRecentProfiles();
    }
}

void ShaderWindow::RegisterHotkeys()
{
    RegisterHotKey(m_mainWindow, HK_FULLSCREEN, MOD_CONTROL | MOD_SHIFT, 0x47); // G
    RegisterHotKey(m_mainWindow, HK_SCREENSHOT, MOD_CONTROL | MOD_SHIFT, 0x53); // S
    RegisterHotKey(m_mainWindow, HK_PAUSE, MOD_CONTROL | MOD_SHIFT, 0x50); // P
}

void ShaderWindow::UnregisterHotkeys()
{
    UnregisterHotKey(m_mainWindow, HK_FULLSCREEN);
    UnregisterHotKey(m_mainWindow, HK_SCREENSHOT);
    UnregisterHotKey(m_mainWindow, HK_PAUSE);
}

void ShaderWindow::Start(_In_ LPWSTR lpCmdLine, HWND paramsWindow, HWND browserWindow, HWND compileWindow)
{
    bool autoStart  = true;
    bool fullScreen = false;

    if(lpCmdLine)
    {
        int  numArgs;
        auto cmdLine = GetCommandLineW();
        auto args    = CommandLineToArgvW(cmdLine, &numArgs);

        for(int a = 1; a < numArgs; a++)
        {
            if(wcscmp(args[a], L"-paused") == 0 || wcscmp(args[a], L"-p") == 0)
                autoStart = false;
            else if(wcscmp(args[a], L"-fullscreen") == 0 || wcscmp(args[a], L"-f") == 0)
                fullScreen = true;
            else if(a == numArgs - 1)
            {
                std::wstring ws(args[a]);
                if(ws.size())
                    LoadProfile(ws);
            }
        }
    }

    m_paramsWindow  = paramsWindow;
    m_browserWindow = browserWindow;
    m_compileWindow = compileWindow;
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

    SetTimer(m_mainWindow, 0, 1000, NULL);
}