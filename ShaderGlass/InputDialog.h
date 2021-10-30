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