#pragma once

#ifdef CAPTURELIB_EXPORTS
#    define CAPTURELIB_API __declspec(dllexport)
#else
#    define CAPTURELIB_API __declspec(dllimport)
#endif

// callback to receive BGRA data
typedef void(__stdcall* CAPTURE_CALLBACK_FUNC)(void* data, UINT width, UINT height, UINT pitch, void* context);

CAPTURELIB_API UINT    CaptureLibVersion();
CAPTURELIB_API HRESULT CaptureLibStart(CAPTURE_CALLBACK_FUNC callbackFunc, UINT type, void* context);
CAPTURELIB_API HRESULT CaptureLibStop();
