/*
ShaderGlass: shader effect overlay
Copyright (C) 2021-2025 mausimus (mausimus.net)
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0
*/

#include "pch.h"

#include "Helpers.h"

#define MAX_WINDOW_TITLE 200

static std::optional<bool> hasCaptureAPI;

wchar_t* convertCharArrayToLPCWSTR(const char* charArray)
{
    wchar_t* wString = new wchar_t[4096];
    MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
    return wString;
}

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

std::wstring GetWindowStringText(HWND hwnd)
{
    wchar_t title[MAX_WINDOW_TITLE];
    GetWindowText(hwnd, title, MAX_WINDOW_TITLE);
    return std::wstring(title);
}

bool HasCaptureAPI()
{
    if(!hasCaptureAPI.has_value())
    {
        try
        {
            hasCaptureAPI = winrt::Windows::Foundation::Metadata::ApiInformation::IsTypePresent(L"Windows.Graphics.Capture.GraphicsCaptureItem");
        }
        catch(...)
        {
            hasCaptureAPI = false;
        }
    }
    return hasCaptureAPI.value();
}

bool Is1903()
{
    try
    {
        return HasCaptureAPI() && !winrt::Windows::Foundation::Metadata::ApiInformation::IsApiContractPresent(L"Windows.Foundation.UniversalApiContract", 9);
    }
    catch(...)
    {
        return false;
    }
}

bool CanDisableBorder()
{
    try
    {
        return HasCaptureAPI() && winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Graphics.Capture.GraphicsCaptureSession", L"IsBorderRequired");
    }
    catch(...)
    {
        return false;
    }
}

bool CanSetCaptureRate()
{
    try
    {
        return HasCaptureAPI() && winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Graphics.Capture.GraphicsCaptureSession", L"MinUpdateInterval");
    }
    catch(...)
    {
        return false;
    }
}

bool CanUpdateCursor()
{
    try
    {
        return HasCaptureAPI() &&
               winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Graphics.Capture.GraphicsCaptureSession", L"IsCursorCaptureEnabled");
    }
    catch(...)
    {
        return false;
    }
}

int32_t GetTicks()
{
    static LARGE_INTEGER freq {.QuadPart = 0};
    static LARGE_INTEGER startTicks {.QuadPart = 0};
    if(freq.QuadPart == 0)
    {
        QueryPerformanceFrequency(&freq);
        QueryPerformanceCounter(&startTicks);
    }

    LARGE_INTEGER ticks;
    if(!QueryPerformanceCounter(&ticks) || freq.QuadPart == 0)
    {
        return GetTickCount64();
    }

    return (int32_t)((ticks.QuadPart - startTicks.QuadPart) / (freq.QuadPart / TICKS_PER_SEC));
}