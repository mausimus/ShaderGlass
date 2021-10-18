#include "pch.h"

#include "resource.h"
#include "ParamsWindow.h"

ParamsWindow::ParamsWindow(CaptureManager& captureManager) :
    m_captureManager(captureManager), m_captureOptions(captureManager.m_options), m_title(), m_windowClass(), m_resetButtonWnd(0),
    m_closeButtonWnd(0)
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
    /*
    AddTrackbar(0, 100, 20, 10);
    AddTrackbar(1000, 2000, 1200, 100);
    */
    m_resetButtonWnd = CreateWindow(L"BUTTON", // Predefined class; Unicode assumed
                                    L"Reset", // Button text
                                    WS_TABSTOP | WS_VISIBLE | WS_CHILD, // Styles
                                    30, // x position
                                    m_trackbars.size() * 50 + 10, // y position
                                    100, // Button width
                                    25, // Button height
                                    m_mainWindow, // Parent window
                                    NULL, // No menu.
                                    (HINSTANCE)GetWindowLongPtr(m_mainWindow, GWLP_HINSTANCE),
                                    NULL); // Pointer not needed.

    m_closeButtonWnd = CreateWindow(L"BUTTON", // Predefined class; Unicode assumed
                                    L"Close", // Button text
                                    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // Styles
                                    200, // x position
                                    m_trackbars.size() * 50 + 10, // y position
                                    100, // Button width
                                    25, // Button height
                                    m_mainWindow, // Parent window
                                    NULL, // No menu.
                                    (HINSTANCE)GetWindowLongPtr(m_mainWindow, GWLP_HINSTANCE),
                                    NULL); // Pointer not needed.

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

    for(const auto& p : m_captureManager.Params())
    {
        AddTrackbar(p->minValue * 100, p->maxValue * 100, p->currentValue * 100, 1, p->name.c_str());
    }

    int yPos = m_trackbars.size() * 50 + 10;
    if(m_trackbars.size())
        SetWindowPos(m_resetButtonWnd, m_mainWindow, 30, yPos, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);
    else
        SetWindowPos(m_resetButtonWnd, m_mainWindow, 30, yPos, 0, 0, SWP_NOSIZE | SWP_HIDEWINDOW);

    SetWindowPos(m_closeButtonWnd, m_mainWindow, 200, yPos, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
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
    case WM_HSCROLL: {
        int id = 0;
        if(lParam != 0)
        {
            id       = GetDlgCtrlID((HWND)lParam);
            auto pos = SendMessage(m_trackbars[id].trackBarWnd, TBM_GETPOS, 0, 0);
            SetWindowText(m_trackbars[id].paramValueWnd, convertCharArrayToLPCWSTR(std::to_string(pos).c_str()));
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
            { }
        }
            return 0;
        }
    }
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

void ParamsWindow::AddTrackbar(UINT        iMin, // minimum value in trackbar range
                               UINT        iMax, // maximum value in trackbar range
                               UINT        iStart,
                               UINT        iStep,
                               const char* name) // maximum value in trackbar selection
{
    auto hwndTrack = CreateWindowEx(0, // no extended styles
                                    TRACKBAR_CLASS, // class name
                                    L"Trackbar Control", // title (caption)
                                    WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS, // style
                                    100,
                                    m_trackbars.size() * 50 + 10, // position
                                    200,
                                    30, // size
                                    m_mainWindow, // parent window
                                    (HMENU)m_trackbars.size() /*ID_TRACKBAR*/, // control identifier
                                    m_instance, // instance
                                    NULL // no WM_CREATE parameter
    );

    SendMessage(hwndTrack,
                TBM_SETRANGE,
                (WPARAM)TRUE, // redraw flag
                (LPARAM)MAKELONG(iMin / iStep, iMax / iStep)); // min. & max. positions

    SendMessage(hwndTrack,
                TBM_SETPOS,
                (WPARAM)TRUE, // redraw flag
                (LPARAM)iStart / iStep);

    const int staticWidth  = 50;
    const int staticHeight = 20;

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

    auto paramValueWnd = CreateWindowEx(0,
                                        L"STATIC",
                                        convertCharArrayToLPCWSTR(std::to_string(iStart/iStep).c_str()),
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

    ParamsTrackbar pt;
    pt.trackBarWnd   = hwndTrack;
    pt.paramNameWnd  = paramNameWnd;
    pt.paramValueWnd = paramValueWnd;

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
