/*
ShaderGlass: shader effect overlay
Copyright (C) 2021-2025 mausimus (mausimus.net)
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0
*/

#include "pch.h"

#include "resource.h"
#include "Helpers.h"
#include "CropDialog.h"
#include "Options.h"

static float   inputValue;
static LPCWSTR labelText;
static LPCWSTR startingValue;
static HWND    s_mainWindow;
static RECT    s_croppedArea;

CropDialog::CropDialog(HINSTANCE hInstance, HWND mainWindow) : m_instance(hInstance), m_mainWindow(mainWindow)
{
    s_mainWindow = m_mainWindow;
}

INT_PTR CALLBACK CropProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
    case WM_INITDIALOG: {

        auto edit1 = GetDlgItem(hDlg, IDC_EDIT1);
        auto edit2 = GetDlgItem(hDlg, IDC_EDIT2);
        auto edit3 = GetDlgItem(hDlg, IDC_EDIT3);
        auto edit4 = GetDlgItem(hDlg, IDC_EDIT4);

        SendDlgItemMessage(hDlg, IDC_SPIN1, UDM_SETBUDDY, (WPARAM)edit1, 0);
        SendDlgItemMessage(hDlg, IDC_SPIN2, UDM_SETBUDDY, (WPARAM)edit2, 0);
        SendDlgItemMessage(hDlg, IDC_SPIN3, UDM_SETBUDDY, (WPARAM)edit3, 0);
        SendDlgItemMessage(hDlg, IDC_SPIN4, UDM_SETBUDDY, (WPARAM)edit4, 0);

        SendDlgItemMessage(hDlg, IDC_SPIN1, UDM_SETRANGE, 0, 4096);
        SendDlgItemMessage(hDlg, IDC_SPIN2, UDM_SETRANGE, 0, 4096);
        SendDlgItemMessage(hDlg, IDC_SPIN3, UDM_SETRANGE, 0, 4096);
        SendDlgItemMessage(hDlg, IDC_SPIN4, UDM_SETRANGE, 0, 4096);

        SendMessage(hDlg, DM_SETDEFID, (WPARAM)IDOK, (LPARAM)0);
        SetDlgItemInt(hDlg, IDC_EDIT1, s_croppedArea.top, false);
        SetDlgItemInt(hDlg, IDC_EDIT2, s_croppedArea.right, false);
        SetDlgItemInt(hDlg, IDC_EDIT3, s_croppedArea.bottom, false);
        SetDlgItemInt(hDlg, IDC_EDIT4, s_croppedArea.left, false);
    }
        return TRUE;

    case WM_COMMAND:
        if(HIWORD(wParam) == EN_CHANGE)
        {
            auto source = LOWORD(wParam);
            int  p      = -1;
            switch(source)
            {
            case IDC_EDIT1:
                p = 0;
                break;
            case IDC_EDIT2:
                p = 1;
                break;
            case IDC_EDIT3:
                p = 2;
                break;
            case IDC_EDIT4:
                p = 3;
                break;
            default:
                break;
            }
            if(p >= 0)
            {
                BOOL translated;
                auto value = GetDlgItemInt(hDlg, source, &translated, false);
                if(translated)
                {
                    PostMessage(s_mainWindow, WM_USER_CROP_UPDATED, p, value);
                }
            }
            break;
        }
        switch(wParam)
        {
        case IDOK:
            EndDialog(hDlg, TRUE);
            return TRUE;

        case IDRESET: {
            s_croppedArea = RECT {0, 0, 0, 0};
            PostMessage(s_mainWindow, WM_USER_CROP_UPDATED, 4, 0);
            SetDlgItemInt(hDlg, IDC_EDIT1, 0, false);
            SetDlgItemInt(hDlg, IDC_EDIT2, 0, false);
            SetDlgItemInt(hDlg, IDC_EDIT3, 0, false);
            SetDlgItemInt(hDlg, IDC_EDIT4, 0, false);
        }
        break;

        case IDCANCEL:
            EndDialog(hDlg, TRUE);
            return TRUE;
        }
        return 0;
    }
    return FALSE;

    UNREFERENCED_PARAMETER(lParam);
}

void CropDialog::Show(RECT croppedArea)
{
    s_croppedArea = croppedArea;
    DialogBox(m_instance, MAKEINTRESOURCE(IDD_CROP_DIALOG), m_mainWindow, CropProc);
}
