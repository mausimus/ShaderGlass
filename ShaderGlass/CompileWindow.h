#pragma once

#define MAX_LOADSTRING 100

#include "Options.h"
#include "CaptureManager.h"
#include "Helpers.h"

class CompileWindow
{
public:
    CompileWindow(CaptureManager& manager);

    bool Create(_In_ HINSTANCE hInstance, _In_ int nCmdShow, _In_ HWND shaderWindow);

    HWND m_mainWindow {nullptr};

private:
    WCHAR                     m_title[MAX_LOADSTRING];
    WCHAR                     m_windowClass[MAX_LOADSTRING];
    HINSTANCE                 m_instance {nullptr};
    HWND                      m_textControl;
    HWND                      m_shaderWindow;
    HFONT                     m_font;
    CaptureManager&           m_captureManager;
    CaptureOptions&           m_captureOptions;
    float                     m_dpiScale;

    void Resize();
    void Build();

    static LRESULT CALLBACK WndProcProxy(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    ATOM                    MyRegisterClass(HINSTANCE hInstance);
    BOOL                    InitInstance(HINSTANCE hInstance, int nCmdShow);
    LRESULT CALLBACK        WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};