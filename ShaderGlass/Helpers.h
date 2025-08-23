/*
ShaderGlass: shader effect overlay
Copyright (C) 2021-2025 mausimus (mausimus.net)
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0
*/

#pragma once

#define MAX_WINDOW_TITLE 200
#define MAX_DEVICE_NAME 200
#define TICKS_PER_SEC 10000
#define REFERENCE_FPS 59.95
#define REFERENCE_FRAME_TIME (TICKS_PER_SEC / REFERENCE_FPS)

wchar_t*     convertCharArrayToLPCWSTR(const char* charArray);
BOOL         IsAltTabWindow(HWND hwnd);
std::wstring GetWindowStringText(HWND hwnd);
bool         HasCaptureAPI();
bool         Is1903();
bool         CanDisableBorder();
bool         CanSetCaptureRate();
bool         CanUpdateCursor();
int32_t      GetTicks();
