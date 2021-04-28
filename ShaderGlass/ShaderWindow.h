#pragma once

#define MAX_LOADSTRING 100

#include "Options.h"
#include "CaptureManager.h"
#include "Helpers.h"

class ShaderWindow
{
public:
    ShaderWindow(CaptureManager& manager);

    bool Create(_In_ HINSTANCE hInstance, _In_ int nCmdShow);
    void Start(_In_ LPWSTR lpCmdLine);

private:
    WCHAR                        m_title[MAX_LOADSTRING];
    WCHAR                        m_windowClass[MAX_LOADSTRING];
    HINSTANCE                    m_instance {nullptr};
    HWND                         m_mainWindow {nullptr};
    HMENU                        m_mainMenu {nullptr};
    HMENU                        m_programMenu {nullptr};
    HMENU                        m_pixelSizeMenu {nullptr};
    HMENU                        m_aspectRatioMenu {nullptr};
    HMENU                        m_outputScaleMenu {nullptr};
    HMENU                        m_frameSkipMenu {nullptr};
    HMENU                        m_shaderMenu {nullptr};
    HMENU                        m_flipMenu {nullptr};
    HMENU                        m_windowMenu {nullptr};
    HMENU                        m_modeMenu {nullptr};
    HMENU                        m_displayMenu {nullptr};
    HMENU                        m_outputWindowMenu {nullptr};
    HMENU                        m_inputMenu {nullptr};
    std::map<std::string, HMENU> m_categoryMenus;
    std::vector<CaptureWindow>   m_captureWindows;
    std::vector<CaptureDisplay>  m_captureDisplays;
    CaptureManager&              m_captureManager;
    CaptureOptions&              m_captureOptions;
    unsigned                     m_numPresets {0};
    unsigned                     m_selectedPixelSize {0};
    unsigned                     m_selectedOutputScale {0};
    unsigned                     m_selectedAspectRatio {0};
    unsigned                     m_selectedFrameSkip {0};
    bool                         m_isTransparent {false};
    bool                         m_isBorderless {false};
    RECT                         m_lastPosition;

    void LoadProfile(const std::string& fileName);
    void LoadProfile();
    void SaveProfile(const std::string& fileName);
    void SaveProfile();
    void ScanWindows();
    void ScanDisplays();
    void BuildInputMenu();
    void BuildOutputMenu();
    void BuildShaderMenu();
    void SetTransparent(bool transparent);
    void AdjustWindowSize(HWND hWnd);
    void UpdateWindowState();
    void ToggleBorderless(HWND hWnd);

    static BOOL CALLBACK    EnumWindowsProcProxy(_In_ HWND hwnd, _In_ LPARAM lParam);
    static BOOL CALLBACK    EnumDisplayMonitorsProcProxy(_In_ HMONITOR hMonitor, _In_ HDC hDC, _In_ LPRECT lpRect, _In_ LPARAM lParam);
    static LRESULT CALLBACK WndProcProxy(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    ATOM                    MyRegisterClass(HINSTANCE hInstance);
    BOOL                    InitInstance(HINSTANCE hInstance, int nCmdShow);
    BOOL CALLBACK           EnumWindowsProc(_In_ HWND hwnd, _In_ LPARAM lParam);
    BOOL CALLBACK           EnumDisplayMonitorsProc(_In_ HMONITOR hMonitor, _In_ HDC hDC, _In_ LPRECT lpRect, _In_ LPARAM lParam);
    LRESULT CALLBACK        WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};