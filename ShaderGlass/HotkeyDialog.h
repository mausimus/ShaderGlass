/*
ShaderGlass: shader effect overlay
Copyright (C) 2021-2025 mausimus (mausimus.net)
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0
*/

#pragma once

class HotkeyDialog
{
public:
    HotkeyDialog(HINSTANCE hInstance, HWND mainWindow);

    WORD GetHotkey(const wchar_t* label, WORD hk);
    std::wstring GetKeyString(WORD hk);

private:
    HINSTANCE m_instance;
    HWND      m_mainWindow;
};