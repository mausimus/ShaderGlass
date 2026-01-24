#pragma once

#ifdef SG_WINE
#define ____FIReference_1_boolean_INTERFACE_DEFINED__
#define memcpy_s(dst, dsize, src, size) memcpy(dst, src, size)
#define swprintf_s(format, size, ...) swprintf(format, __VA_ARGS__)
#define _ReadWriteBarrier()
#define assert(x) ;
#endif

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#define OEMRESOURCE
// Windows Header Files
#include <windows.h>
#include <shellapi.h>
#include <commdlg.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <dwmapi.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <mutex>
#include <vector>
#include <map>
#include <climits>
#include <cmath>
#include <filesystem>

#include <unknwn.h>
#include <inspectable.h>

#include <windows.foundation.h>
#include <windows.foundation.metadata.h>
#include <windows.graphics.capture.h>
#include <windows.graphics.capture.interop.h>

#include "winrt/base.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi1_6.h>
#include <windows.graphics.directx.direct3d11.h>

#ifdef SG_WINE

inline int wcstombs_s(
   size_t *pReturnValue,
   char *mbstr,
   const wchar_t *wcstr,
   size_t count
)
   {
    *pReturnValue = std::wcstombs(mbstr, wcstr, count);
    return 0;
   }
#define _strnicmp strncmp
#define _snwprintf_s swprintf
#define wcscat_s(dest, size, src) wcscat(dest, src)
#define wcsncpy_s wcsncpy
#define WDA_EXCLUDEFROMCAPTURE 0

#endif
