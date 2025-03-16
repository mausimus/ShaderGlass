#include "pch.h"

#include "resource.h"
#include "CompileWindow.h"

constexpr int WINDOW_WIDTH  = 400;
constexpr int WINDOW_HEIGHT = 100;

CompileWindow::CompileWindow(CaptureManager& captureManager) :
    m_captureManager(captureManager), m_captureOptions(captureManager.m_options), m_title(), m_windowClass(), m_font(0), m_dpiScale(1.0f)
{ }

LRESULT CALLBACK CompileWindow::WndProcProxy(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    CompileWindow* app;
    if(msg == WM_CREATE)
    {
        app = (CompileWindow*)(((LPCREATESTRUCT)lParam)->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)app);
    }
    else
    {
        app = (CompileWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    }
    return app->WndProc(hWnd, msg, wParam, lParam);
}

ATOM CompileWindow::MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc   = CompileWindow::WndProcProxy;
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

BOOL CompileWindow::InitInstance(HINSTANCE hInstance, int nCmdShow)
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
                              WS_EX_TOOLWINDOW,
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

    Build();

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    SetWindowPos(m_mainWindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    return TRUE;
}

void CompileWindow::Resize()
{
    RECT rcClient;
    GetClientRect(m_mainWindow, &rcClient);
    SetWindowPos(m_textControl, NULL, 0, 0, rcClient.right, rcClient.bottom, 0);
}

void CompileWindow::Build()
{
    RECT rcClient; // dimensions of client area
    GetClientRect(m_mainWindow, &rcClient);
    m_textControl = CreateWindowEx(0,
                                   L"STATIC", TEXT("Compiling shader...\r\n"), SS_CENTER | SS_CENTERIMAGE | WS_VISIBLE | WS_CHILD,
                                   0,
                                   0,
                                   rcClient.right,
                                   rcClient.bottom,
                                   m_mainWindow,
                                   NULL,
                                   m_instance,
                                   NULL);
    SendMessage(m_textControl, WM_SETFONT, (LPARAM)m_font, true);
    Resize();
}

LRESULT CALLBACK CompileWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
    case WM_CLOSE: {
        ShowWindow(hWnd, SW_HIDE);
        return 0;
    }
    case WM_SIZE: {
        Resize();
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
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

bool CompileWindow::Create(_In_ HINSTANCE hInstance, _In_ int nCmdShow, _In_ HWND shaderWindow)
{
    m_shaderWindow = shaderWindow;

    LoadStringW(hInstance, IDS_APP_TITLE, m_title, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_COMPILEWINDOW, m_windowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    if(!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    return TRUE;
}
