/*
ShaderGlass: shader effect overlay
Copyright (C) 2021-2026 mausimus (mausimus.net)
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0
*/

#pragma once

#define CAPTURELIB_API __stdcall

// callback to receive BGRx data
typedef void(__stdcall* CAPTURE_CALLBACK_FUNC)(void* data, UINT width, UINT height, UINT pitch, void* context);

// internal callback from PW
typedef void(* PIPEWIRE_CALLBACK_FUNC)(void* data, int width, int height, int pitch);

CAPTURELIB_API UINT CaptureLibVersion();
CAPTURELIB_API HRESULT CaptureLibInit();
CAPTURELIB_API HRESULT CaptureLibStart(UINT type, UINT cursor, CAPTURE_CALLBACK_FUNC callbackFunc, void* context);
CAPTURELIB_API HRESULT CaptureLibStop();
