/*
ShaderGlass: shader effect overlay
Copyright (C) 2021-2025 mausimus (mausimus.net)
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0
*/

#pragma once

class CropDialog
{
public:
    CropDialog(HINSTANCE hInstance, HWND mainWindow);

    void Show(RECT croppedArea);

private:
    HINSTANCE m_instance;
    HWND      m_mainWindow;
};