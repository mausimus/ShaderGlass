#include "pch.h"

#include "CaptureLib.h"

static CAPTURE_CALLBACK_FUNC sCallbackFunc = NULL;
static int                   sActive       = 0;
static void*                 sContext      = NULL;

static DWORD* sData    = NULL;
static int    sFrameNo = 0;

static DWORD __stdcall ThreadFunc(LPVOID data)
{
    do
    {
        Sleep(10);
        int o = 0;
        for(int y = 0; y < 480; y++)
            for(int x = 0; x < 640; x++)
                sData[o++] = 0xff000000 + (((x + sFrameNo) % 255) << 16) + (((x + y) % 255) << 8) + 0xff;
        sCallbackFunc(sData, 640, 480, 640 * 4, sContext);
        sFrameNo++;
    } while(sActive);
    free(sData);
    sData         = NULL;
    sCallbackFunc = NULL;
    sContext      = NULL;
    return 0;
}

CAPTURELIB_API UINT CaptureLibVersion()
{
    return 1;
}

CAPTURELIB_API HRESULT CaptureLibInit()
{
    return S_OK;
}

CAPTURELIB_API HRESULT CaptureLibStart(UINT type, UINT cursor, CAPTURE_CALLBACK_FUNC callbackFunc, void* context)
{
    if(sActive)
        return E_FAIL;
    sCallbackFunc = callbackFunc;
    sContext      = context;
    sActive       = 1;

    sData = (DWORD*)malloc(640 * 480 * 4);

    CreateThread(NULL, 0, ThreadFunc, NULL, 0, NULL);
    return S_OK;
}

CAPTURELIB_API HRESULT CaptureLibStop()
{
    if(!sActive)
        return E_FAIL;
    sActive = 0;
    return S_OK;
}
