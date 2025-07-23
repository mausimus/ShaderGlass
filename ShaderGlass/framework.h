#pragma once

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
#include <mutex>

#include <Unknwn.h>
#include <inspectable.h>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Metadata.h>
#include <winrt/Windows.Graphics.Capture.h>
#include <windows.graphics.capture.interop.h>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi1_6.h>
#include <winrt/windows.graphics.directx.direct3d11.h>
