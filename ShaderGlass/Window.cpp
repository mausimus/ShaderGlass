#include "pch.h"

#include "resource.h"
#include "Options.h"
#include "CaptureManager.h"
#include "Helpers.h"

#define MAX_LOADSTRING 100

HINSTANCE                    hInst;
WCHAR                        szTitle[MAX_LOADSTRING];
WCHAR                        szWindowClass[MAX_LOADSTRING];
HWND                         mainWindow;
HMENU                        mainMenu;
HMENU                        programMenu;
HMENU                        pixelSizeMenu;
HMENU                        aspectRatioMenu;
HMENU                        outputScaleMenu;
HMENU                        frameSkipMenu;
HMENU                        shaderMenu;
HMENU                        flipMenu;
HMENU                        windowMenu;
HMENU                        modeMenu;
HMENU                        displayMenu;
HMENU                        outputWindowMenu;
ATOM                         MyRegisterClass(HINSTANCE hInstance);
BOOL                         InitInstance(HINSTANCE, int);
LRESULT CALLBACK             WndProc(HWND, UINT, WPARAM, LPARAM);
std::map<std::string, HMENU> categoryMenus;
std::vector<CaptureWindow>   captureWindows;
CaptureOptions               options;
CaptureManager               captureManager(options);
unsigned                     numPresets;
unsigned                     selectedPixelSize;
unsigned                     selectedOutputScale;
unsigned                     selectedAspectRatio;
bool                         isTransparent = false;

// See http://blogs.msdn.com/b/oldnewthing/archive/2007/10/08/5351207.aspx
BOOL IsAltTabWindow(HWND hwnd)
{
    // Start at the root owner
    HWND hwndWalk = GetAncestor(hwnd, GA_ROOTOWNER);

    // See if we are the last active visible popup
    HWND hwndTry;
    while((hwndTry = GetLastActivePopup(hwndWalk)) != hwndTry)
    {
        if(IsWindowVisible(hwndTry))
            break;
        hwndWalk = hwndTry;
    }
    return hwndWalk == hwnd;
}

std::string GetWindowStringText(HWND hwnd)
{
    int                  len = GetWindowTextLength(hwnd) + 1;
    std::vector<wchar_t> buf(len);
    GetWindowText(hwnd, &buf[0], len);
    std::wstring wide = &buf[0];
    std::string  s(wide.begin(), wide.end());
    return s;
}

BOOL CALLBACK EnumWindowsProc(_In_ HWND hwnd, _In_ LPARAM /*lParam*/)
{
    if(captureWindows.size() >= MAX_CAPTURE_WINDOWS)
        return false;

    if(hwnd != mainWindow && IsWindowVisible(hwnd) && IsAltTabWindow(hwnd))
    {
        DWORD isCloaked;
        DWORD size = sizeof(isCloaked);
        DwmGetWindowAttribute(hwnd, DWMWA_CLOAKED, &isCloaked, size);

        if(!isCloaked)
        {
            TITLEBARINFO ti;
            ti.cbSize = sizeof(ti);
            GetTitleBarInfo(hwnd, &ti);
            if(ti.rgstate[0] & STATE_SYSTEM_INVISIBLE)
                return true;

            if(GetWindowLong(hwnd, GWL_EXSTYLE) & WS_EX_TOOLWINDOW)
                return true;

            CaptureWindow cw(hwnd, GetWindowStringText(hwnd));
            if(cw.name.size())
                captureWindows.emplace_back(cw);
        }
    }
    return true;
}

void ScanWindows()
{
    captureWindows.clear();
    EnumWindows(&EnumWindowsProc, 0);

    for(UINT i = 0; i < MAX_CAPTURE_WINDOWS; i++)
    {
        RemoveMenu(windowMenu, WM_CAPTURE_WINDOW(i), MF_BYCOMMAND);
    }
    UINT i = 0;
    for(const auto& w : captureWindows)
    {
        AppendMenu(windowMenu, MF_STRING, WM_CAPTURE_WINDOW(i++), convertCharArrayToLPCWSTR(w.name.c_str()));
        if(options.captureWindow == w.hwnd)
            CheckMenuItem(windowMenu, WM_CAPTURE_WINDOW(i - 1), MF_CHECKED | MF_BYCOMMAND);
    }
}

void BuildInputMenu()
{
    auto sMenu    = GetSubMenu(mainMenu, 1);
    pixelSizeMenu = CreatePopupMenu();
    AppendMenu(pixelSizeMenu, MF_STRING, IDM_PIXELSIZE_NEXT, L"Next\tp");
    for(const auto& px : pixelSizes)
    {
        AppendMenu(pixelSizeMenu, MF_STRING, px.first, convertCharArrayToLPCWSTR(px.second.text));
    }
    AppendMenu(sMenu, MF_STRING | MF_POPUP, (UINT_PTR)pixelSizeMenu, L"Pixel Size");

    displayMenu = GetSubMenu(sMenu, 0);
    windowMenu  = GetSubMenu(sMenu, 1);
}

void BuildOutputMenu()
{
    auto sMenu = GetSubMenu(mainMenu, 2);
    DeleteMenu(sMenu, 0, MF_BYPOSITION);

    modeMenu = GetSubMenu(sMenu, 0);
    outputWindowMenu = GetSubMenu(sMenu, 1);
    flipMenu         = GetSubMenu(sMenu, 2);

    outputScaleMenu = CreatePopupMenu();
    for(const auto& os : outputScales)
    {
        AppendMenu(outputScaleMenu, MF_STRING, os.first, os.second.text);
    }
    AppendMenu(sMenu, MF_STRING | MF_POPUP, (UINT_PTR)outputScaleMenu, L"Scale");

    aspectRatioMenu = CreatePopupMenu();
    for(const auto& ar : aspectRatios)
    {
        AppendMenu(aspectRatioMenu, MF_STRING, ar.first, ar.second.text);
    }
    AppendMenu(sMenu, MF_STRING | MF_POPUP, (UINT_PTR)aspectRatioMenu, L"Aspect Ratio Correction");

    frameSkipMenu = CreatePopupMenu();
    for(const auto& fs : frameSkips)
    {
        AppendMenu(frameSkipMenu, MF_STRING, fs.first, fs.second.text);
    }
    AppendMenu(sMenu, MF_STRING | MF_POPUP, (UINT_PTR)frameSkipMenu, L"Frame Skip");
}

void BuildShaderMenu()
{
    shaderMenu = GetSubMenu(mainMenu, 3);
    DeleteMenu(shaderMenu, 0, MF_BYPOSITION);
    unsigned i = 0;

    for(const auto& sp : captureManager.Presets())
    {
        if(strcmp(sp->Category, "general") == 0)
        {
            AppendMenu(shaderMenu, MF_STRING, WM_SHADER(i++), convertCharArrayToLPCWSTR(sp->Name));
            continue;
        }
        if(categoryMenus.find(sp->Category) == categoryMenus.end())
        {
            categoryMenus.insert(std::make_pair(sp->Category, CreatePopupMenu()));
        }
        auto menu = categoryMenus.find(sp->Category)->second;
        AppendMenu(menu, MF_STRING, WM_SHADER(i++), convertCharArrayToLPCWSTR(sp->Name));
    }

    for(auto m : categoryMenus)
    {
        AppendMenu(shaderMenu, MF_STRING | MF_POPUP, (UINT_PTR)m.second, convertCharArrayToLPCWSTR(m.first.c_str()));
    }
    numPresets = i;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SHADERGLASS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    if(!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SHADERGLASS));
    MSG    msg;

    captureManager.Initialize();

    mainMenu = LoadMenu(hInstance, MAKEINTRESOURCEW(IDC_SHADERGLASS));

    programMenu = GetSubMenu(mainMenu, 0);
    BuildInputMenu();
    BuildOutputMenu();
    BuildShaderMenu();
    ScanWindows();
    SetMenu(mainWindow, mainMenu);
    srand(static_cast<unsigned>(time(NULL)));

    options.monitor      = nullptr;
    options.outputWindow = mainWindow;

    // set defaults
    SendMessage(mainWindow, WM_COMMAND, WM_PIXEL_SIZE(2), 0);
    SendMessage(mainWindow, WM_COMMAND, WM_ASPECT_RATIO(0), 0);
    SendMessage(mainWindow, WM_COMMAND, WM_SHADER(20), 0);
    SendMessage(mainWindow, WM_COMMAND, WM_FRAME_SKIP(1), 0);
    SendMessage(mainWindow, WM_COMMAND, WM_OUTPUT_SCALE(0), 0);
    SendMessage(mainWindow, WM_COMMAND, IDM_MODE_GLASS, 0);

    // auto-start
    SendMessage(mainWindow, WM_COMMAND, IDM_START, 0);

    // Main message loop:
    while(GetMessage(&msg, nullptr, 0, 0))
    {
        if(!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc   = WndProc;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = 0;
    wcex.hInstance     = hInstance;
    wcex.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SHADERGLASS));
    wcex.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = CreateSolidBrush(0x00000000);
    wcex.lpszMenuName  = MAKEINTRESOURCEW(IDC_SHADERGLASS);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm       = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    //wcex.hCursor = NULL;

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;

    RECT rect;
    rect.left   = 0;
    rect.top    = 0;
    rect.right  = 960;
    rect.bottom = 600;
    AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, true, WS_EX_WINDOWEDGE);

    HWND hWnd = CreateWindowW(szWindowClass,
                              szTitle,
                              WS_OVERLAPPEDWINDOW | WS_EX_WINDOWEDGE,
                              CW_USEDEFAULT,
                              CW_USEDEFAULT,
                              rect.right - rect.left,
                              rect.bottom - rect.top,
                              nullptr,
                              nullptr,
                              hInstance,
                              nullptr);

    if(!hWnd)
    {
        return FALSE;
    }

    mainWindow = hWnd;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

void SetTransparent(bool transparent)
{
    if(transparent != isTransparent)
    {
        LONG cur_style = GetWindowLong(mainWindow, GWL_EXSTYLE);
        if(transparent)
            SetWindowLong(mainWindow, GWL_EXSTYLE, cur_style | WS_EX_TRANSPARENT);
        else
            SetWindowLong(mainWindow, GWL_EXSTYLE, cur_style & ~WS_EX_TRANSPARENT);
        isTransparent = transparent;
    }
}

void AdjustWindowSize(HWND hWnd)
{
    if(captureManager.IsActive() && options.captureWindow && options.clone)
    {
        // resize client area to captured window x scale
        RECT captureRect;
        GetClientRect(options.captureWindow, &captureRect);

        RECT r;
        GetClientRect(hWnd, &r);

        auto requiredW = static_cast<LONG>(captureRect.right * options.outputScale / options.aspectRatio);
        auto requiredH = static_cast<LONG>(captureRect.bottom * options.outputScale);
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
    else
    {
        float xAlign = options.pixelWidth * options.outputScale;
        float yAlign = options.pixelHeight * options.outputScale;
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
                AdjustWindowRect(&clientRect, GetWindowLong(hWnd, GWL_STYLE), TRUE);

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

void UpdateWindowState()
{
    // always topmost when processing
    if(captureManager.IsActive())
        SetWindowPos(mainWindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    else
        SetWindowPos(mainWindow, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    LONG cur_style = GetWindowLong(mainWindow, GWL_EXSTYLE);
    if(captureManager.IsActive() && options.transparent)
        // active desktop glass mode - layered/transparent window
        SetWindowLong(mainWindow, GWL_EXSTYLE, cur_style | WS_EX_LAYERED);
    else
        // clone or normal window
        SetWindowLong(mainWindow, GWL_EXSTYLE, cur_style & ~WS_EX_LAYERED);

    if(captureManager.IsActive() && !options.clone && !options.captureWindow)
        // desktop glass - exclude from capture
        SetWindowDisplayAffinity(mainWindow, WDA_EXCLUDEFROMCAPTURE);
    else
        SetWindowDisplayAffinity(mainWindow, WDA_NONE);

    // update title
    if(captureManager.IsActive())
    {
        const auto& pixelSize   = pixelSizes.at(WM_PIXEL_SIZE(selectedPixelSize));
        const auto& outputScale = outputScales.at(WM_OUTPUT_SCALE(selectedOutputScale));
        const auto& aspectRatio = aspectRatios.at(WM_ASPECT_RATIO(selectedAspectRatio));
        const auto& shader      = captureManager.Presets().at(options.presetNo);

        char windowName[26];
        windowName[0] = 0;
        if(options.captureWindow)
        {
            auto captureTitle = GetWindowStringText(options.captureWindow);
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
        SetWindowTextA(mainWindow, title);
    }
    else
    {
        SetWindowTextA(mainWindow, "ShaderGlass");
    }

    AdjustWindowSize(mainWindow);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
    case WM_COMMAND: {
        UINT wmId = LOWORD(wParam);
        switch(wmId)
        {
        case IDM_START: {
            if(options.captureWindow && !IsWindow(options.captureWindow))
                return 0;
            captureManager.StartSession();
            EnableMenuItem(programMenu, IDM_START, MF_BYCOMMAND | MF_DISABLED);
            EnableMenuItem(programMenu, IDM_STOP, MF_BYCOMMAND | MF_ENABLED);
            UpdateWindowState();
        }
        break;
        case IDM_TOGGLEMENU:
            if(GetMenu(hWnd))
                SetMenu(hWnd, NULL);
            else
                SetMenu(hWnd, mainMenu);
            break;
        case IDM_SHADER_NEXT:
            SendMessage(hWnd, WM_COMMAND, WM_SHADER((options.presetNo + 1) % numPresets), 0);
            break;
        case IDM_SHADER_RANDOM:
            SendMessage(hWnd, WM_COMMAND, WM_SHADER(rand() % numPresets), 0);
            break;
        case IDM_PIXELSIZE_NEXT:
            SendMessage(hWnd, WM_COMMAND, WM_PIXEL_SIZE((selectedPixelSize + 1) % pixelSizes.size()), 0);
            break;
        case IDM_FLIP_HORIZONTAL:
            options.flipHorizontal = !options.flipHorizontal;
            if(options.flipHorizontal)
                CheckMenuItem(flipMenu, IDM_FLIP_HORIZONTAL, MF_CHECKED | MF_BYCOMMAND);
            else
                CheckMenuItem(flipMenu, IDM_FLIP_HORIZONTAL, MF_UNCHECKED | MF_BYCOMMAND);
            captureManager.UpdateOutputFlip();
            break;
        case IDM_FLIP_VERTICAL:
            options.flipVertical = !options.flipVertical;
            if(options.flipVertical)
                CheckMenuItem(flipMenu, IDM_FLIP_VERTICAL, MF_CHECKED | MF_BYCOMMAND);
            else
                CheckMenuItem(flipMenu, IDM_FLIP_VERTICAL, MF_UNCHECKED | MF_BYCOMMAND);
            captureManager.UpdateOutputFlip();
            break;
        case IDM_WINDOW_SOLID:
            options.transparent = false;
            CheckMenuItem(outputWindowMenu, IDM_WINDOW_TRANSPARENT, MF_UNCHECKED | MF_BYCOMMAND);
            CheckMenuItem(outputWindowMenu, IDM_WINDOW_SOLID, MF_CHECKED | MF_BYCOMMAND);
            UpdateWindowState();
            break;
        case IDM_WINDOW_TRANSPARENT:
            options.transparent = true;
            CheckMenuItem(outputWindowMenu, IDM_WINDOW_TRANSPARENT, MF_CHECKED | MF_BYCOMMAND);
            CheckMenuItem(outputWindowMenu, IDM_WINDOW_SOLID, MF_UNCHECKED | MF_BYCOMMAND);
            UpdateWindowState();
            break;
        case IDM_MODE_GLASS:
            options.clone = false;
            CheckMenuItem(modeMenu, IDM_MODE_GLASS, MF_CHECKED | MF_BYCOMMAND);
            CheckMenuItem(modeMenu, IDM_MODE_CLONE, MF_UNCHECKED | MF_BYCOMMAND);
            options.transparent = true;
            CheckMenuItem(outputWindowMenu, IDM_WINDOW_TRANSPARENT, MF_CHECKED | MF_BYCOMMAND);
            CheckMenuItem(outputWindowMenu, IDM_WINDOW_SOLID, MF_UNCHECKED | MF_BYCOMMAND);
            captureManager.UpdateInput();
            UpdateWindowState();
            break;
        case IDM_MODE_CLONE:
            options.clone = true;
            CheckMenuItem(modeMenu, IDM_MODE_GLASS, MF_UNCHECKED | MF_BYCOMMAND);
            CheckMenuItem(modeMenu, IDM_MODE_CLONE, MF_CHECKED | MF_BYCOMMAND);
            options.transparent = false;
            CheckMenuItem(outputWindowMenu, IDM_WINDOW_TRANSPARENT, MF_UNCHECKED | MF_BYCOMMAND);
            CheckMenuItem(outputWindowMenu, IDM_WINDOW_SOLID, MF_CHECKED | MF_BYCOMMAND);
            captureManager.UpdateInput();
            UpdateWindowState();
            break;
        case IDM_WINDOW_SCAN:
            ScanWindows();
            break;
        case IDM_DISPLAY_ALLDISPLAYS:
            CheckMenuRadioItem(
                windowMenu, WM_CAPTURE_WINDOW(0), WM_CAPTURE_WINDOW(static_cast<UINT>(captureWindows.size())), 0, MF_BYCOMMAND);
            CheckMenuItem(displayMenu, IDM_DISPLAY_ALLDISPLAYS, MF_CHECKED | MF_BYCOMMAND);
            options.captureWindow = NULL;
            options.clone         = false;
            options.transparent   = true;
            CheckMenuItem(modeMenu, IDM_MODE_GLASS, MF_CHECKED | MF_BYCOMMAND);
            CheckMenuItem(modeMenu, IDM_MODE_CLONE, MF_UNCHECKED | MF_BYCOMMAND);
            CheckMenuItem(outputWindowMenu, IDM_WINDOW_TRANSPARENT, MF_CHECKED | MF_BYCOMMAND);
            CheckMenuItem(outputWindowMenu, IDM_WINDOW_SOLID, MF_UNCHECKED | MF_BYCOMMAND);
            captureManager.UpdateInput();
            UpdateWindowState();
            break;
        case IDM_STOP: {
            captureManager.StopSession();
            EnableMenuItem(programMenu, IDM_STOP, MF_BYCOMMAND | MF_DISABLED);
            EnableMenuItem(programMenu, IDM_START, MF_BYCOMMAND | MF_ENABLED);
            UpdateWindowState();
        }
        break;
        case IDM_EXIT:
            captureManager.StopSession();
            DestroyWindow(hWnd);
            break;
        case IDM_ABOUT1:
        case IDM_ABOUT2:
        case IDM_ABOUT3:
#ifdef _DEBUG
            captureManager.Debug();
#else
            ShellExecute(0, 0, L"https://github.com/rohatsu/ShaderGlass", 0, 0, SW_SHOW);
#endif
            break;
        default:
            if(wmId >= WM_USER && wmId <= 0x7FFF)
            {
                if(wmId >= WM_SHADER(0) && wmId < WM_SHADER(MAX_SHADERS))
                {
                    CheckMenuItem(shaderMenu, wmId, MF_CHECKED | MF_BYCOMMAND);
                    CheckMenuItem(shaderMenu, options.presetNo + WM_SHADER(0), MF_UNCHECKED | MF_BYCOMMAND);
                    options.presetNo = wmId - WM_SHADER(0);
                    captureManager.UpdateShaderPreset();
                    UpdateWindowState();
                    break;
                }
                if(wmId >= WM_CAPTURE_WINDOW(0) && wmId < WM_CAPTURE_WINDOW(MAX_CAPTURE_WINDOWS))
                {
                    CheckMenuRadioItem(
                        windowMenu, WM_CAPTURE_WINDOW(0), WM_CAPTURE_WINDOW(static_cast<UINT>(captureWindows.size())), wmId, MF_BYCOMMAND);
                    CheckMenuItem(displayMenu, IDM_DISPLAY_ALLDISPLAYS, MF_UNCHECKED | MF_BYCOMMAND);
                    options.captureWindow = captureWindows.at(wmId - WM_CAPTURE_WINDOW(0)).hwnd;
                    options.clone         = true;
                    options.transparent   = false;
                    CheckMenuItem(modeMenu, IDM_MODE_GLASS, MF_UNCHECKED | MF_BYCOMMAND);
                    CheckMenuItem(modeMenu, IDM_MODE_CLONE, MF_CHECKED | MF_BYCOMMAND);
                    CheckMenuItem(outputWindowMenu, IDM_WINDOW_TRANSPARENT, MF_UNCHECKED | MF_BYCOMMAND);
                    CheckMenuItem(outputWindowMenu, IDM_WINDOW_SOLID, MF_CHECKED | MF_BYCOMMAND);
                    captureManager.UpdateInput();
                    UpdateWindowState();
                    break;
                }
                const auto& pixelSize = pixelSizes.find(wmId);
                if(pixelSize != pixelSizes.end())
                {
                    selectedPixelSize = wmId - WM_PIXEL_SIZE(0);
                    CheckMenuRadioItem(
                        pixelSizeMenu, WM_PIXEL_SIZE(0), WM_PIXEL_SIZE(static_cast<UINT>(pixelSizes.size() - 1)), wmId, MF_BYCOMMAND);
                    options.pixelWidth  = pixelSize->second.w;
                    options.pixelHeight = pixelSize->second.h;
                    captureManager.UpdatePixelSize();
                    UpdateWindowState();
                    break;
                }
                const auto& outputScale = outputScales.find(wmId);
                if(outputScale != outputScales.end())
                {
                    selectedOutputScale = wmId - WM_OUTPUT_SCALE(0);
                    CheckMenuRadioItem(
                        outputScaleMenu, 0, static_cast<UINT>(outputScales.size()), wmId - WM_OUTPUT_SCALE(0), MF_BYPOSITION);
                    options.outputScale = outputScale->second.s;
                    captureManager.UpdateOutputSize();
                    UpdateWindowState();
                    break;
                }
                const auto& aspectRatio = aspectRatios.find(wmId);
                if(aspectRatio != aspectRatios.end())
                {
                    selectedAspectRatio = wmId - WM_ASPECT_RATIO(0);
                    CheckMenuRadioItem(
                        aspectRatioMenu, 0, static_cast<UINT>(aspectRatios.size()), wmId - WM_ASPECT_RATIO(0), MF_BYPOSITION);
                    options.aspectRatio = aspectRatio->second.r;
                    captureManager.UpdateOutputSize();
                    UpdateWindowState();
                    break;
                }
                const auto& frameSkip = frameSkips.find(wmId);
                if(frameSkip != frameSkips.end())
                {
                    CheckMenuRadioItem(frameSkipMenu, 0, static_cast<UINT>(frameSkips.size()), wmId - WM_FRAME_SKIP(0), MF_BYPOSITION);
                    options.frameSkip = frameSkip->second.s;
                    captureManager.UpdateFrameSkip();
                    break;
                }
            }
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_SIZE: {
        switch (wParam)
        {
        case SIZE_MINIMIZED:
            if(captureManager.IsActive())
            {
                options.paused = true;
                captureManager.StopSession();
            }
            break;
        case SIZE_MAXIMIZED:
        case SIZE_RESTORED:
            if(options.paused)
            {
                captureManager.StartSession();
                options.paused = false;
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
        if(captureManager.IsActive() && options.captureWindow)
        {
            // if we click in our client area, active the captured window
            SetForegroundWindow(options.captureWindow);
        }
        break;
    }
    case WM_PAINT: {
        if(captureManager.IsActive() && options.transparent)
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
    case WM_DESTROY:
        captureManager.StopSession();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
