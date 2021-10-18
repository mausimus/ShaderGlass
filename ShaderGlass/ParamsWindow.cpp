#include "pch.h"

#include "resource.h"
#include "ParamsWindow.h"

const int staticWidth  = 200;
const int staticHeight = 20;

const int buttonWidth  = 100;
const int buttonHeight = 25;

const int buttonTop = 20;
const int paramsTop = 75;

const int windowWidth  = 500;
const int windowHeight = 500;

const int trackWidth  = 200;
const int trackHeight = 30;

ParamsWindow::ParamsWindow(CaptureManager& captureManager) :
    m_captureManager(captureManager), m_captureOptions(captureManager.m_options), m_title(), m_windowClass(), m_resetButtonWnd(0),
    m_closeButtonWnd(0), m_font(0)
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
    //(HBRUSH) GetStockObject(GRAY_BRUSH);
    //CreateSolidBrush(RGB(200, 200, 200));
    wcex.lpszMenuName  = 0;
    wcex.lpszClassName = m_windowClass;
    wcex.hIconSm       = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    //wcex.hCursor = NULL;

    return RegisterClassExW(&wcex);
}

BOOL ParamsWindow::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    m_instance = hInstance;

    RECT rect;
    rect.left   = 0;
    rect.top    = 0;
    rect.right  = windowWidth;
    rect.bottom = windowHeight;
    AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, true, WS_EX_WINDOWEDGE);

    HWND hWnd = CreateWindowW(m_windowClass,
                              m_title,
                              WS_OVERLAPPEDWINDOW | WS_EX_WINDOWEDGE | WS_VSCROLL,
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

    NONCLIENTMETRICS metrics = {};
    metrics.cbSize           = sizeof(metrics);
    SystemParametersInfo(SPI_GETNONCLIENTMETRICS, metrics.cbSize, &metrics, 0);

    m_font = CreateFontIndirect(&metrics.lfCaptionFont);

    // When you're done with the font, don't forget to call
    //    DeleteObject(guiFont);

    m_mainWindow = hWnd;
    /*
    AddTrackbar(0, 100, 20, 10);
    AddTrackbar(1000, 2000, 1200, 100);
    */
    m_resetButtonWnd = CreateWindow(L"BUTTON", // Predefined class; Unicode assumed
                                    L"Reset", // Button text
                                    WS_TABSTOP | WS_VISIBLE | WS_CHILD, // Styles
                                    (windowWidth / 3) - (buttonWidth / 2), // x position
                                    buttonTop, //m_trackbars.size() * 50 + 10, // y position
                                    buttonWidth, // Button width
                                    buttonHeight, // Button height
                                    m_mainWindow, // Parent window
                                    NULL, // No menu.
                                    (HINSTANCE)GetWindowLongPtr(m_mainWindow, GWLP_HINSTANCE),
                                    NULL); // Pointer not needed.
    SendMessage(m_resetButtonWnd, WM_SETFONT, (LPARAM)m_font, true);

    m_closeButtonWnd = CreateWindow(L"BUTTON", // Predefined class; Unicode assumed
                                    L"Close", // Button text
                                    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // Styles
                                    (2 * windowWidth / 3) - (buttonWidth / 2), // x position
                                    buttonTop, //m_trackbars.size() * 50 + 10, // y position
                                    buttonWidth, // Button width
                                    buttonHeight, // Button height
                                    m_mainWindow, // Parent window
                                    NULL, // No menu.
                                    (HINSTANCE)GetWindowLongPtr(m_mainWindow, GWLP_HINSTANCE),
                                    NULL); // Pointer not needed.
    SendMessage(m_closeButtonWnd, WM_SETFONT, (LPARAM)m_font, true);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    SetWindowPos(m_mainWindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    return TRUE;
}

void ParamsWindow::RebuildControls()
{
    for(auto& t : m_trackbars)
    {
        DestroyWindow(t.paramNameWnd);
        DestroyWindow(t.paramValueWnd);
        DestroyWindow(t.trackBarWnd);
    }
    m_trackbars.clear();

    for(const auto& pt : m_captureManager.Params())
    {
        const auto& p = std::get<1>(pt);
        if(p->size == 4 && p->name != "FrameCount" && p->maxValue != p->minValue)
        {
            int numSteps = 10;            
            if (p->stepValue != 0)
            {
                numSteps = (int)roundf((p->maxValue - p->minValue) / p->stepValue);
            }            
            int startValue = (int)roundf(numSteps * (p->currentValue - p->minValue) / (p->maxValue - p->minValue));
            AddTrackbar(0,
                        numSteps,
                        startValue,
                        numSteps,
                        p->name.c_str(),
                        p);
            /* AddTrackbar(p->minValue * 100,
                        p->maxValue * 100,
                        p->currentValue * 100,
                        p->stepValue != 0 ? p->stepValue * 100 : 10,
                        p->name.c_str(),
                        p);*/
        }
    }

    //int yPos = m_trackbars.size() * 50 + 10;
    if(m_trackbars.size())
        SetWindowPos(m_resetButtonWnd,
                     m_mainWindow,
                     (windowWidth / 3) - (buttonWidth / 2),
                     buttonTop /* yPos*/,
                     0,
                     0,
                     SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);
    else
        SetWindowPos(
            m_resetButtonWnd, m_mainWindow, (windowWidth / 3) - (buttonWidth / 2), buttonTop /* yPos*/, 0, 0, SWP_NOSIZE | SWP_HIDEWINDOW);

    SetWindowPos(
        m_closeButtonWnd, m_mainWindow, (2 * windowWidth / 3) - (buttonWidth / 2), buttonTop /* yPos*/, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
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
    case WM_CLOSE: {
        ShowWindow(hWnd, SW_HIDE);
        return 0;
    }
    case WM_VSCROLL: {
        SCROLLINFO si;
        si.cbSize = sizeof(si);
        si.fMask  = SIF_ALL;
        GetScrollInfo(hWnd, SB_VERT, &si);

        // Save the position for comparison later on.
        int yPos = si.nPos;
        switch(LOWORD(wParam))
        {

        // User clicked the HOME keyboard key.
        case SB_TOP:
            si.nPos = si.nMin;
            break;

        // User clicked the END keyboard key.
        case SB_BOTTOM:
            si.nPos = si.nMax;
            break;

        // User clicked the top arrow.
        case SB_LINEUP:
            si.nPos -= 1;
            break;

        // User clicked the bottom arrow.
        case SB_LINEDOWN:
            si.nPos += 1;
            break;

        // User clicked the scroll bar shaft above the scroll box.
        case SB_PAGEUP:
            si.nPos -= si.nPage;
            break;

        // User clicked the scroll bar shaft below the scroll box.
        case SB_PAGEDOWN:
            si.nPos += si.nPage;
            break;

        // User dragged the scroll box.
        case SB_THUMBTRACK:
            si.nPos = si.nTrackPos;
            break;

        default:
            break;
        }

        // Set the position and then retrieve it.  Due to adjustments
        // by Windows it may not be the same as the value set.
        si.fMask = SIF_POS;
        SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
        GetScrollInfo(hWnd, SB_VERT, &si);

        // If the position has changed, scroll window and update it.
        if(si.nPos != yPos)
        {
            ScrollWindow(hWnd, 0, 5 /* yChar*/ * (yPos - si.nPos), NULL, NULL);
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
            auto  p     = m_trackbars[id].param;

            float value = p->minValue + (p->maxValue - p->minValue) * pos / m_trackbars[id].steps;

            SetWindowText(m_trackbars[id].paramValueWnd, convertCharArrayToLPCWSTR(std::to_string(value).c_str()));

            m_trackbars[id].param->currentValue = value;

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
                for(const auto& t : m_trackbars)
                    auto pos = SendMessage(t.trackBarWnd, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)t.def);

                m_captureManager.ResetParams();
            }
        }
            return 0;
        }
    }
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

void ParamsWindow::AddTrackbar(UINT         iMin, // minimum value in trackbar range
                               UINT         iMax, // maximum value in trackbar range
                               UINT         iStart,
                               UINT         iSteps,
                               const char*  name,
                               ShaderParam* p) // maximum value in trackbar selection
{
    auto hwndTrack = CreateWindowEx(0, // no extended styles
                                    TRACKBAR_CLASS, // class name
                                    L"Trackbar Control", // title (caption)
                                    WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS, // style
                                    staticWidth,
                                    m_trackbars.size() * 50 + paramsTop, // position
                                    trackWidth,
                                    trackHeight, // size
                                    m_mainWindow, // parent window
                                    (HMENU)m_trackbars.size() /*ID_TRACKBAR*/, // control identifier
                                    m_instance, // instance
                                    NULL // no WM_CREATE parameter
    );

    SendMessage(hwndTrack,
                TBM_SETRANGE,
                (WPARAM)TRUE, // redraw flag
                (LPARAM)MAKELONG(iMin, iMax)); // min. & max. positions

    SendMessage(hwndTrack,
                TBM_SETPOS,
                (WPARAM)TRUE, // redraw flag
                (LPARAM)iStart);

    SendMessage(hwndTrack, WM_SETFONT, (LPARAM)m_font, true);

    auto paramNameWnd = CreateWindowEx(0,
                                       L"STATIC",
                                       convertCharArrayToLPCWSTR(name),
                                       SS_RIGHT | WS_CHILD | WS_VISIBLE,
                                       0,
                                       0,
                                       staticWidth,
                                       staticHeight,
                                       m_mainWindow,
                                       NULL,
                                       m_instance,
                                       NULL);
    SendMessage(hwndTrack, TBM_SETBUDDY, (WPARAM)TRUE, (LPARAM)paramNameWnd);
    SendMessage(paramNameWnd, WM_SETFONT, (LPARAM)m_font, true);

    float value = p->minValue + (p->maxValue - p->minValue) * iStart / iSteps;

    auto paramValueWnd = CreateWindowEx(0,
                                        L"STATIC",
                                        convertCharArrayToLPCWSTR(std::to_string(value).c_str()),
                                        SS_LEFT | WS_CHILD | WS_VISIBLE,
                                        0,
                                        0,
                                        staticWidth,
                                        staticHeight,
                                        m_mainWindow,
                                        NULL,
                                        m_instance,
                                        NULL);
    SendMessage(hwndTrack, TBM_SETBUDDY, (WPARAM)FALSE, (LPARAM)paramValueWnd);
    SendMessage(paramValueWnd, WM_SETFONT, (LPARAM)m_font, true);

    ParamsTrackbar pt;
    pt.trackBarWnd   = hwndTrack;
    pt.paramNameWnd  = paramNameWnd;
    pt.paramValueWnd = paramValueWnd;
    pt.def           = iStart;
    pt.steps         = iSteps;
    pt.param         = p;

    m_trackbars.emplace_back(pt);
}

bool ParamsWindow::Create(_In_ HINSTANCE hInstance, _In_ int nCmdShow)
{
    LoadStringW(hInstance, IDS_PARAM_TITLE, m_title, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SHADERPARAMS, m_windowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    if(!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    return TRUE;
}
