/*
ShaderGlass: shader effect overlay
Copyright (C) 2021-2025 mausimus (mausimus.net)
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0
*/

#pragma once

class InputDialog
{
public:
    InputDialog(HINSTANCE hInstance, HWND mainWindow);

    float GetInput(const std::string& label, float existing);

private:
    HINSTANCE m_instance;
    HWND      m_mainWindow;
};
