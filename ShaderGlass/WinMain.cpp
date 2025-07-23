/*
ShaderGlass: shader effect overlay
Copyright (C) 2021-2025 mausimus (mausimus.net)
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0
*/

#include "pch.h"

#include "resource.h"
#include "ShaderWindow.h"
#include "ParamsWindow.h"
#include "BrowserWindow.h"
#include "CompileWindow.h"

#pragma comment(                                                                                                                                                                   \
    linker,                                                                                                                                                                        \
    "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    if(!HasCaptureAPI())
    {
        MessageBox(NULL, L"Windows Capture API not present! Only file input will be possible.", L"ShaderGlass", MB_OK | MB_ICONERROR);
    }

    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SHADERGLASS));
    MSG    msg;

    INITCOMMONCONTROLSEX ccs;
    ccs.dwICC  = ICC_STANDARD_CLASSES;
    ccs.dwSize = sizeof(ccs);
    InitCommonControlsEx(&ccs);

    CaptureManager captureManager(hInstance);
    ShaderWindow   shaderWindow(captureManager);
    if(!shaderWindow.Create(hInstance, nCmdShow))
    {
        return FALSE;
    }

    ParamsWindow paramsWindow(captureManager);
    if(!paramsWindow.Create(hInstance, SW_HIDE, shaderWindow.m_mainWindow))
    {
        return FALSE;
    }

    BrowserWindow browserWindow(captureManager);
    if(!browserWindow.Create(hInstance, SW_HIDE, shaderWindow.m_mainWindow, paramsWindow.m_mainWindow))
    {
        return FALSE;
    }

    CompileWindow compileWindow(captureManager);
    if(!compileWindow.Create(hInstance, SW_HIDE, shaderWindow.m_mainWindow))
    {
        return FALSE;
    }

    shaderWindow.Start(lpCmdLine, paramsWindow.m_mainWindow, browserWindow.m_mainWindow, compileWindow.m_mainWindow);

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