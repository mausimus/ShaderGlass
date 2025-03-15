#pragma once

#define MAX_LOADSTRING 100

#include "Options.h"
#include "CaptureManager.h"
#include "Helpers.h"

class BrowserWindow
{
public:
    BrowserWindow(CaptureManager& manager);

    bool Create(_In_ HINSTANCE hInstance, _In_ int nCmdShow, _In_ HWND shaderWindow, _In_ HWND paramsWindow);

    HWND m_mainWindow {nullptr};

private:
    WCHAR                     m_title[MAX_LOADSTRING];
    WCHAR                     m_windowClass[MAX_LOADSTRING];
    HINSTANCE                 m_instance {nullptr};
    HWND                      m_treeControl;
    HWND                      m_shaderWindow;
    HWND                      m_paramsWindow;
    HFONT                     m_font;
    CaptureManager&           m_captureManager;
    CaptureOptions&           m_captureOptions;
    float                     m_dpiScale;
    int                       g_nOpen;
    int                       g_nClosed;
    int                       g_nDocument;
    std::map<UINT, HTREEITEM> m_items;
    std::map<UINT, HTREEITEM> m_favorites;
    HTREEITEM                 m_imported;

    void Resize();
    void Build();

    static LRESULT CALLBACK WndProcProxy(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    ATOM                    MyRegisterClass(HINSTANCE hInstance);
    BOOL                    InitInstance(HINSTANCE hInstance, int nCmdShow);
    LRESULT CALLBACK        WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    BOOL      CreateImageList(HWND hwndTV);
    HTREEITEM BrowserWindow::AddItemToTree(HWND hwndTV, LPTSTR lpszItem, LPARAM lParam, int nLevel);
};