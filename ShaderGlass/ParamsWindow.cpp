#include "pch.h"

#include "resource.h"
#include "ParamsWindow.h"

constexpr int STATIC_WIDTH  = 230;
constexpr int STATIC_HEIGHT = 40;
constexpr int BUTTON_WIDTH  = 100;
constexpr int BUTTON_HEIGHT = 25;
constexpr int BUTTON_TOP    = 20;
constexpr int PARAMS_TOP    = 75;
constexpr int PARAM_HEIGHT  = 40;
constexpr int WINDOW_WIDTH  = 600;
constexpr int WINDOW_HEIGHT = 600;
constexpr int TRACK_WIDTH   = 200;
constexpr int TRACK_HEIGHT  = 30;        

ParamsWindow::ParamsWindow(CaptureManager& captureManager) :
    m_captureManager(captureManager), m_captureOptions(captureManager.m_options), m_title(), m_windowClass(), m_resetButtonWnd(0),
    m_closeButtonWnd(0), m_font(0), m_dpiScale(1.0f), m_hwndTip(NULL)
{ }

LRESULT CALLBACK ParamsWindow::WndProcProxy(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    ParamsWindow* app;
    if(msg == WM_CREATE)
    {
        app = (ParamsWindow*)(((LPCREATESTRUCT)lParam)->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)app);
    }
    else
    {
        app = (ParamsWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    }
    return app->WndProc(hWnd, msg, wParam, lParam);
}

ATOM ParamsWindow::MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc   = ParamsWindow::WndProcProxy;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = 0;
    wcex.hInstance     = hInstance;
    wcex.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SHADERGLASS));
    wcex.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetSysColorBrush(COLOR_MENU);
    wcex.lpszMenuName  = 0;
    wcex.lpszClassName = m_windowClass;
    wcex.hIconSm       = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL ParamsWindow::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    m_instance = hInstance;

    m_dpiScale = GetDpiForSystem() / 96.0f;

    RECT rect;
    rect.left   = 0;
    rect.top    = 0;
    rect.right  = WINDOW_WIDTH * m_dpiScale;
    rect.bottom = WINDOW_HEIGHT * m_dpiScale;
    AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, true, WS_EX_WINDOWEDGE);

    HWND hWnd = CreateWindowW(m_windowClass,
                              m_title,
                              WS_OVERLAPPEDWINDOW | WS_EX_WINDOWEDGE | WS_VSCROLL,
                              CW_USEDEFAULT,
                              CW_USEDEFAULT,
                              rect.right - rect.left,
                              rect.bottom - rect.top,
                              m_shaderWindow,
                              nullptr,
                              hInstance,
                              this);

    if(!hWnd)
    {
        return FALSE;
    }

    NONCLIENTMETRICS metrics = {};
    metrics.cbSize           = sizeof(metrics);
    SystemParametersInfo(SPI_GETNONCLIENTMETRICS, metrics.cbSize, &metrics, 0);

    m_font       = CreateFontIndirect(&metrics.lfCaptionFont);
    m_mainWindow = hWnd;

    m_resetButtonWnd = CreateWindow(L"BUTTON",
                                    L"Defaults",
                                    WS_TABSTOP | WS_VISIBLE | WS_CHILD,
                                    m_dpiScale * ((WINDOW_WIDTH / 3) - (BUTTON_WIDTH / 2)),
                                    m_dpiScale * BUTTON_TOP,
                                    m_dpiScale * BUTTON_WIDTH,
                                    m_dpiScale * BUTTON_HEIGHT,
                                    m_mainWindow,
                                    NULL,
                                    (HINSTANCE)GetWindowLongPtr(m_mainWindow, GWLP_HINSTANCE),
                                    NULL);
    SendMessage(m_resetButtonWnd, WM_SETFONT, (LPARAM)m_font, true);

    m_closeButtonWnd = CreateWindow(L"BUTTON",
                                    L"Close",
                                    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                                    m_dpiScale * ((2 * WINDOW_WIDTH / 3) - (BUTTON_WIDTH / 2)),
                                    m_dpiScale * BUTTON_TOP,
                                    m_dpiScale * BUTTON_WIDTH,
                                    m_dpiScale * BUTTON_HEIGHT,
                                    m_mainWindow,
                                    NULL,
                                    (HINSTANCE)GetWindowLongPtr(m_mainWindow, GWLP_HINSTANCE),
                                    NULL);
    SendMessage(m_closeButtonWnd, WM_SETFONT, (LPARAM)m_font, true);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    SetWindowPos(m_mainWindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    return TRUE;
}

void ParamsWindow::ResizeScrollBar()
{
    RECT rect;
    GetClientRect(m_mainWindow, &rect);

    SCROLLINFO si;
    si.cbSize = sizeof(si);
    si.fMask  = SIF_ALL;
    GetScrollInfo(m_mainWindow, SB_VERT, &si);

    // calculate
    auto actualSize   = rect.bottom - rect.top;
    auto requiredSize = m_dpiScale * ((m_trackbars.size() + 0) * PARAM_HEIGHT + PARAMS_TOP);

    if(requiredSize < actualSize)
    {
        ScrollWindow(m_mainWindow, 0, 5 * si.nPos, NULL, NULL);
        si.nMin = 0;
        si.nMax = 0;
        si.nPos = 0;
        SetScrollInfo(m_mainWindow, SB_VERT, &si, true);
        EnableScrollBar(m_mainWindow, SB_VERT, ESB_DISABLE_BOTH);
    }
    else
    {
        EnableScrollBar(m_mainWindow, SB_VERT, ESB_ENABLE_BOTH);
        ScrollWindow(m_mainWindow, 0, 5 * si.nPos, NULL, NULL);
        si.nMin = 0;
        si.nMax = (requiredSize - actualSize) / 5;
        si.nPos = 0;
        SetScrollInfo(m_mainWindow, SB_VERT, &si, true);
    }
}

void ParamsWindow::RebuildControls()
{
    for(auto& t : m_trackbars)
    {
        DestroyWindow(t.paramNameWnd);
        DestroyWindow(t.paramValueWnd);
        DestroyWindow(t.trackBarWnd);
    }
    if(m_hwndTip)
    {
        DestroyWindow(m_hwndTip);
    }
    m_trackbars.clear();

    SCROLLINFO si;
    si.cbSize = sizeof(si);
    si.fMask  = SIF_ALL;
    GetScrollInfo(m_mainWindow, SB_VERT, &si);
    if(si.nPos != 0)
    {
        ResizeScrollBar();
    }

    char title[200];
    const auto& shader = m_captureManager.Presets().at(m_captureOptions.presetNo);
    if(m_captureManager.IsActive())
        snprintf(title, 200, "Shader Parameters: %s", shader->Name.c_str());
    else
        snprintf(title, 200, "Shader Parameters");
    SetWindowTextA(m_mainWindow, title);

    m_hwndTip = CreateWindowEx(NULL,
                             TOOLTIPS_CLASS,
                             NULL,
                             WS_POPUP | TTS_NOANIMATE | TTS_NOFADE | TTS_NOPREFIX,
                             CW_USEDEFAULT,
                             CW_USEDEFAULT,
                             CW_USEDEFAULT,
                             CW_USEDEFAULT,
                             m_mainWindow,
                             NULL,
                             m_instance,
                             NULL);

    for(const auto& pt : m_captureManager.Params())
    {
        const auto& p = std::get<1>(pt);
        if(p->maxValue != p->minValue)
        {
            int numSteps = 10;
            if(p->stepValue != 0)
            {
                numSteps = (int)roundf((p->maxValue - p->minValue) / p->stepValue);
            }
            int startValue = (int)roundf(numSteps * (p->currentValue - p->minValue) / (p->maxValue - p->minValue));
            AddTrackbar(0, numSteps, startValue, numSteps, p->name.c_str(), p);
        }
    }

    if(m_trackbars.size())
        SetWindowPos(m_resetButtonWnd,
                     m_mainWindow,
                     m_dpiScale * ((WINDOW_WIDTH / 3) - (BUTTON_WIDTH / 2)),
                     m_dpiScale * BUTTON_TOP,
                     0,
                     0,
                     SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);
    else
        ShowWindow(m_resetButtonWnd, SW_HIDE);

    SetWindowPos(m_closeButtonWnd,
                 m_mainWindow,
                 m_dpiScale * ((2 * WINDOW_WIDTH / 3) - (BUTTON_WIDTH / 2)),
                 m_dpiScale * BUTTON_TOP,
                 0,
                 0,
                 SWP_NOSIZE | SWP_NOZORDER);

    ResizeScrollBar();
}

LRESULT CALLBACK ParamsWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
    case WM_SHOWWINDOW: {
        if(wParam)
        {
            // showing
            RebuildControls();
        }
        break;
    }
    case WM_KEYDOWN: {
        if(wParam == VK_ESCAPE)
        {
            ShowWindow(m_mainWindow, SW_HIDE);
            return 1;
        }
        break;
    }
    case WM_CLOSE: {
        ShowWindow(hWnd, SW_HIDE);
        return 0;
    }
    case WM_SIZE: {
        ResizeScrollBar();
        break;
    }
    case WM_MOUSEWHEEL:
    case WM_VSCROLL: {
        SCROLLINFO si;
        si.cbSize = sizeof(si);
        si.fMask  = SIF_ALL;
        GetScrollInfo(hWnd, SB_VERT, &si);

        int yPos = si.nPos;
        if(message == WM_MOUSEWHEEL)
        {
            si.nPos -= PARAM_HEIGHT * GET_WHEEL_DELTA_WPARAM(wParam) / 120 / 4;
        }
        else
            switch(LOWORD(wParam))
            {
            case SB_TOP:
                si.nPos = si.nMin;
                break;
            case SB_BOTTOM:
                si.nPos = si.nMax;
                break;
            case SB_LINEUP:
                si.nPos -= 1;
                break;
            case SB_LINEDOWN:
                si.nPos += 1;
                break;
            case SB_PAGEUP:
                si.nPos -= si.nPage;
                break;
            case SB_PAGEDOWN:
                si.nPos += si.nPage;
                break;
            case SB_THUMBTRACK:
                si.nPos = si.nTrackPos;
                break;
            default:
                break;
            }

        si.fMask = SIF_POS;
        SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
        GetScrollInfo(hWnd, SB_VERT, &si);

        if(si.nPos != yPos)
        {
            ScrollWindow(hWnd, 0, 5 * (yPos - si.nPos), NULL, NULL);
            UpdateWindow(hWnd);
        }

        return 0;
    }
    case WM_HSCROLL: {
        int id = 0;
        if(lParam != 0)
        {
            id       = GetDlgCtrlID((HWND)lParam);
            auto pos = SendMessage(m_trackbars[id].trackBarWnd, TBM_GETPOS, 0, 0);
            auto p   = *m_trackbars[id].params.begin();

            float value = p->minValue + (p->maxValue - p->minValue) * pos / m_trackbars[id].steps;

            SetWindowText(m_trackbars[id].paramValueWnd, convertCharArrayToLPCWSTR(std::to_string(value).c_str()));

            for(auto& tp : m_trackbars[id].params)
            {
                tp->currentValue = value;
            }

            m_captureManager.UpdateParams();
        }
        return 0;
    }
    case WM_COMMAND: {
        UINT wmId = LOWORD(wParam);
        switch(wmId)
        {
        case BN_CLICKED: {
            if(lParam == (UINT)m_closeButtonWnd)
            {
                ShowWindow(m_mainWindow, SW_HIDE);
            }
            else if(lParam == (UINT)m_resetButtonWnd)
            {
                m_captureManager.ResetParams();
                RebuildControls();
            }
            return 0;
        }
        case IDM_UPDATE_PARAMS: {
            if(IsWindowVisible(m_mainWindow))
            {
                RebuildControls();
            }
        }
            return 0;
        }
    }
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

void ParamsWindow::AddTrackbar(UINT iMin, UINT iMax, UINT iStart, UINT iSteps, const char* name, ShaderParam* p)
{
    // de-dupe parameters
    for(auto& t : m_trackbars)
    {
        if(strcmp(t.paramName, name) == 0 && t.def == iStart && t.steps == iSteps)
        {
            t.params.push_back(p);
            return;
        }
    }

    auto hwndTrack = CreateWindowEx(0,
                                    TRACKBAR_CLASS,
                                    L"Trackbar Control",
                                    WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
                                    m_dpiScale * STATIC_WIDTH,
                                    m_dpiScale * (m_trackbars.size() * PARAM_HEIGHT + PARAMS_TOP),
                                    m_dpiScale * TRACK_WIDTH,
                                    m_dpiScale * TRACK_HEIGHT,
                                    m_mainWindow,
                                    (HMENU)m_trackbars.size(),
                                    m_instance,
                                    NULL);

    SendMessage(hwndTrack,
                TBM_SETRANGE,
                (WPARAM)TRUE, // redraw flag
                (LPARAM)MAKELONG(iMin, iMax));

    SendMessage(hwndTrack,
                TBM_SETPOS,
                (WPARAM)TRUE, // redraw flag
                (LPARAM)iStart);

    SendMessage(hwndTrack, WM_SETFONT, (LPARAM)m_font, true);

    const char* label = p->description.size() ? p->description.c_str() : name;
    const char* tooltip = label; //p->description.size() ? name : p->description.c_str();

    auto paramNameWnd = CreateWindowEx(0,
                                       L"STATIC",
                                       convertCharArrayToLPCWSTR(label),
                                       SS_RIGHT | SS_NOTIFY | WS_CHILD | WS_VISIBLE,
                                       2,
                                       0,
                                       m_dpiScale * STATIC_WIDTH - 2,
                                       m_dpiScale * STATIC_HEIGHT,
                                       m_mainWindow,
                                       NULL,
                                       m_instance,
                                       NULL);
    SendMessage(hwndTrack, TBM_SETBUDDY, (WPARAM)TRUE, (LPARAM)paramNameWnd);
    SendMessage(paramNameWnd, WM_SETFONT, (LPARAM)m_font, true);

    // tooltip
    if(strlen(tooltip) != 0)
    {
        // Associate the tooltip with the tool.
        TOOLINFO toolInfo = {0};
        toolInfo.cbSize   = sizeof(toolInfo);
        toolInfo.hwnd     = m_mainWindow;
        toolInfo.uFlags   = TTF_IDISHWND | TTF_SUBCLASS;
        toolInfo.uId      = (UINT_PTR)paramNameWnd;
        toolInfo.lpszText = convertCharArrayToLPCWSTR(tooltip);
        SendMessage(m_hwndTip, TTM_ADDTOOL, 0, (LPARAM)&toolInfo);
    }

    float value = p->minValue + (p->maxValue - p->minValue) * iStart / iSteps;

    auto paramValueWnd = CreateWindowEx(0,
                                        L"STATIC",
                                        convertCharArrayToLPCWSTR(std::to_string(value).c_str()),
                                        SS_LEFT | WS_CHILD | WS_VISIBLE,
                                        0,
                                        0,
                                        m_dpiScale * STATIC_WIDTH,
                                        m_dpiScale * STATIC_HEIGHT,
                                        m_mainWindow,
                                        NULL,
                                        m_instance,
                                        NULL);
    SendMessage(hwndTrack, TBM_SETBUDDY, (WPARAM)FALSE, (LPARAM)paramValueWnd);
    SendMessage(paramValueWnd, WM_SETFONT, (LPARAM)m_font, true);

    ParamsTrackbar pt;
    pt.paramName     = name;
    pt.trackBarWnd   = hwndTrack;
    pt.paramNameWnd  = paramNameWnd;
    pt.paramValueWnd = paramValueWnd;
    pt.def           = iStart;
    pt.steps         = iSteps;
    pt.params.push_back(p);

    m_trackbars.emplace_back(pt);
}

bool ParamsWindow::Create(_In_ HINSTANCE hInstance, _In_ int nCmdShow, _In_ HWND shaderWindow)
{
    m_shaderWindow = shaderWindow;

    LoadStringW(hInstance, IDS_PARAM_TITLE, m_title, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SHADERPARAMS, m_windowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    if(!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    return TRUE;
}
