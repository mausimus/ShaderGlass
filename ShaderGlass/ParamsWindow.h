#pragma once

#define MAX_LOADSTRING 100

#include "Options.h"
#include "CaptureManager.h"
#include "Helpers.h"

struct ParamsTrackbar
{
    std::string paramName;
    float       min;
    float       max;
    float       step;
    float       val;

    UINT        controlId;
    HWND        trackBarWnd;
    HWND        paramNameWnd;
    HWND        paramValueWnd;
};

class ParamsWindow
{
public:
    ParamsWindow(CaptureManager& manager);

    bool Create(_In_ HINSTANCE hInstance, _In_ int nCmdShow);

    HWND m_mainWindow {nullptr};

private:
    WCHAR           m_title[MAX_LOADSTRING];
    WCHAR           m_windowClass[MAX_LOADSTRING];
    HINSTANCE       m_instance {nullptr};
    HWND            m_resetButtonWnd;
    HWND            m_closeButtonWnd;
    CaptureManager& m_captureManager;
    CaptureOptions& m_captureOptions;

    std::vector<ParamsTrackbar> m_trackbars;

    void AddTrackbar(UINT iMin, UINT iMax, UINT iStart, UINT iStep, const char* name);
    void RebuildControls();

    static LRESULT CALLBACK WndProcProxy(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    ATOM                    MyRegisterClass(HINSTANCE hInstance);
    BOOL                    InitInstance(HINSTANCE hInstance, int nCmdShow);
    LRESULT CALLBACK        WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};