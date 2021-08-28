#pragma once

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

inline std::string GetWindowStringText(HWND hwnd)
{
    int                  len = GetWindowTextLength(hwnd) + 1;
    std::vector<wchar_t> buf(len);
    GetWindowText(hwnd, &buf[0], len);
    std::wstring wide = &buf[0];
    std::string  s(wide.begin(), wide.end());
    return s;
}

inline bool Is1903()
{
    return !winrt::Windows::Foundation::Metadata::ApiInformation::IsApiContractPresent(L"Windows.Foundation.UniversalApiContract", 9);
}

inline bool CanDisableBorder()
{
    return winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(L"Windows.Graphics.Capture.GraphicsCaptureSession", L"IsBorderRequired");
}