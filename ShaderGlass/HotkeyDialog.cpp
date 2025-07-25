/*
ShaderGlass: shader effect overlay
Copyright (C) 2021-2025 mausimus (mausimus.net)
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0
*/

#include "pch.h"

#include "resource.h"
#include "Helpers.h"
#include "HotkeyDialog.h"

static float   inputValue;
static LPCWSTR labelText;
static WORD    nhk;

HotkeyDialog::HotkeyDialog(HINSTANCE hInstance, HWND mainWindow) : m_instance(hInstance), m_mainWindow(mainWindow) { }

static INT_PTR CALLBACK InputProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
    case WM_INITDIALOG:
        SetWindowText(GetDlgItem(hDlg, IDC_STATIC_LABEL), labelText);
        SendDlgItemMessage(hDlg, IDC_HOTKEY1, HKM_SETHOTKEY, nhk, 0);
        PostMessage(hDlg, WM_NEXTDLGCTL, (WPARAM)GetDlgItem(hDlg, IDC_HOTKEY1), TRUE);
        return TRUE;

    case WM_COMMAND:
        switch(wParam)
        {
        case IDOK:
            nhk = LOWORD(SendDlgItemMessage(hDlg, IDC_HOTKEY1, HKM_GETHOTKEY, (WPARAM)0, (LPARAM)0));
            EndDialog(hDlg, TRUE);
            return TRUE;

        case IDCANCEL:
            EndDialog(hDlg, TRUE);
            return TRUE;
        }
        return 0;
    }
    return FALSE;

    UNREFERENCED_PARAMETER(lParam);
}

WORD HotkeyDialog::GetHotkey(const wchar_t* name, WORD hk)
{
    BYTE mod = HIBYTE(hk);
    BYTE vk  = LOBYTE(hk);

    BYTE hkMod = 0;
    if(mod & MOD_ALT)
        hkMod |= HOTKEYF_ALT;
    if(mod & MOD_CONTROL)
        hkMod |= HOTKEYF_CONTROL;
    if(mod & MOD_SHIFT)
        hkMod |= HOTKEYF_SHIFT;
    nhk = MAKEWORD(vk, hkMod);

    labelText = name;
    if(DialogBox(m_instance, MAKEINTRESOURCE(IDD_HOTKEY_DIALOG), m_mainWindow, InputProc) == IDOK)
    {
        vk    = LOBYTE(nhk);
        hkMod = HIBYTE(nhk);

        mod = 0;
        if(hkMod & HOTKEYF_ALT)
            mod |= MOD_ALT;
        if(hkMod & HOTKEYF_CONTROL)
            mod |= MOD_CONTROL;
        if(hkMod & HOTKEYF_SHIFT)
            mod |= MOD_SHIFT;

        return MAKEWORD(vk, mod);
    }

    return hk;
}
