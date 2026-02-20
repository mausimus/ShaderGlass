/*
ShaderGlass: shader effect overlay
Copyright (C) 2021-2026 mausimus (mausimus.net)
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0
*/

#include "framework.h"

#include "WineCap.h"
#include "screencast.h"
#include <gio/gio.h>

static CAPTURE_CALLBACK_FUNC sCallbackFunc = NULL;
static int sInit = 0;
static int sActive = 0;
static void *sContext = NULL;
static int sType = 0;

static void PipeWireCallback(void *data, int width, int height, int pitch)
{
    if (!sActive)
        return;

    sCallbackFunc(data, width, height, pitch, sContext);
}

static __stdcall DWORD ScreenCastThreadFunc(LPVOID ptr)
{
    screencast_run();
    debug("ScreenCastThreadFunc EXIT");
    return 0;
}

CAPTURELIB_API UINT WINECAP_CaptureLibVersion()
{
    return 1;
}

CAPTURELIB_API HRESULT WINECAP_CaptureLibInit()
{
    if (screencast_init() < 0)
    {
        warn("Failed to init ScreenCast");
        return E_FAIL;
    }
    info("ScreenCast initialized!");
    return S_OK;
}

CAPTURELIB_API HRESULT WINECAP_CaptureLibStart(UINT type, UINT cursor, CAPTURE_CALLBACK_FUNC callbackFunc, void *context)
{
    info("Start");
    if (sActive)
        return E_FAIL;

    sCallbackFunc = callbackFunc;
    sContext = context;
    sType = type;
    sActive = 1;

    if (!sInit)
    {
        sInit = 1;
        CreateThread(NULL, 0, ScreenCastThreadFunc, NULL, 0, NULL);
    }
    screencast_start(type, cursor, PipeWireCallback);
    return S_OK;
}

CAPTURELIB_API HRESULT WINECAP_CaptureLibStop()
{
    info("Stop");
    if (!sActive)
        return E_FAIL;
    sActive = 0;

    screencast_stop();
    return S_OK;
}

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD ul_reason_for_call,
                      LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        screencast_destroy();
        break;
    }
    return TRUE;
}
