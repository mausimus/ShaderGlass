#include "pch.h"

#include "resource.h"
#include "Helpers.h"
#include "InputDialog.h"

static float   inputValue;
static LPCWSTR labelText;
static LPCWSTR startingValue;

InputDialog::InputDialog(HINSTANCE hInstance, HWND mainWindow) : m_instance(hInstance), m_mainWindow(mainWindow) { }

INT_PTR CALLBACK InputProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    TCHAR lpszInput[16];
    WORD  cchInput;

    switch(message)
    {
    case WM_INITDIALOG:
        SendMessage(hDlg, DM_SETDEFID, (WPARAM)IDCANCEL, (LPARAM)0);
        SetWindowText(GetDlgItem(hDlg, IDC_STATIC_LABEL), labelText);
        SetWindowText(GetDlgItem(hDlg, IDC_EDIT1), startingValue);
        PostMessage(hDlg, WM_NEXTDLGCTL, (WPARAM)0, (LPARAM)FALSE);
        PostMessage(hDlg, WM_NEXTDLGCTL, (WPARAM)0, (LPARAM)FALSE);
        return TRUE;

    case WM_COMMAND:
        // Set the default push button to "OK" when the user enters text.
        if(HIWORD(wParam) == EN_CHANGE && LOWORD(wParam) == IDC_EDIT1)
        {
            SendMessage(hDlg, DM_SETDEFID, (WPARAM)IDOK, (LPARAM)0);
        }
        switch(wParam)
        {
        case IDOK:
            // Get number of characters.
            cchInput = (WORD)SendDlgItemMessage(hDlg, IDC_EDIT1, EM_LINELENGTH, (WPARAM)0, (LPARAM)0);
            if(cchInput >= 16)
            {
                MessageBox(hDlg, L"Too many characters.", L"Error", MB_OK);

                EndDialog(hDlg, FALSE);
                return FALSE;
            }
            else if(cchInput == 0)
            {
                EndDialog(hDlg, FALSE);
                return FALSE;
            }

            // Put the number of characters into first word of buffer.
            *((LPWORD)lpszInput) = cchInput;

            // Get the characters.
            SendDlgItemMessage(hDlg,
                               IDC_EDIT1,
                               EM_GETLINE,
                               (WPARAM)0, // line 0
                               (LPARAM)lpszInput);

            // Null-terminate the string.
            lpszInput[cchInput] = 0;
            try
            {
                inputValue = std::stof(lpszInput);
            }
            catch(std::exception& e)
            {
                MessageBox(hDlg, L"Invalid value entered.", L"Error", MB_OK);
                EndDialog(hDlg, FALSE);
                return TRUE;
            }

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

float InputDialog::GetInput(const std::string& name, float value)
{
    inputValue    = value;
    labelText     = convertCharArrayToLPCWSTR(name.c_str());
    startingValue = convertCharArrayToLPCWSTR(std::to_string(value).c_str());
    if(DialogBox(m_instance, MAKEINTRESOURCE(IDD_INPUT_DIALOG), m_mainWindow, InputProc) == IDOK)
        return inputValue;
    return NAN;
}
