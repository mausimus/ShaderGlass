#pragma once

#define MAX_WINDOW_TITLE 200

inline wchar_t* convertCharArrayToLPCWSTR(const char* charArray)
{
    wchar_t* wString = new wchar_t[4096];
    MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
    return wString;
}

// See http://blogs.msdn.com/b/oldnewthing/archive/2007/10/08/5351207.aspx
inline BOOL IsAltTabWindow(HWND hwnd)
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

inline std::wstring GetWindowStringText(HWND hwnd)
{
    wchar_t title[MAX_WINDOW_TITLE];
    GetWindowText(hwnd, title, MAX_WINDOW_TITLE);
    return std::wstring(title);
}

inline bool Is1903()
{
    return !winrt::Windows::Foundation::Metadata::ApiInformation::IsApiContractPresent(L"Windows.Foundation.UniversalApiContract", 9);
}

inline bool CanDisableBorder()
{
    return winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Graphics.Capture.GraphicsCaptureSession", L"IsBorderRequired");
}

inline bool CanSetCaptureRate()
{
    return winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Graphics.Capture.GraphicsCaptureSession", L"MinUpdateInterval");
}