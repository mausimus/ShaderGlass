/*
ShaderGlass: shader effect overlay
Copyright (C) 2021-2025 mausimus (mausimus.net)
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0
*/

#include "pch.h"

#include "resource.h"
#include "CursorEmulator.h"

// clang-format off
const std::map<int, std::wstring> cursors = {
    {OCR_NORMAL, L"Arrow"},
    {OCR_IBEAM, L"IBeam"},
    {OCR_WAIT, L"Wait"},
    {OCR_CROSS, L"Crosshair"},
    {OCR_UP, L"UpArrow"},
    {OCR_SIZE, L"SizeAll"},
    {OCR_ICON, L"Arrow"},
    {OCR_SIZENWSE, L"SizeNWSE"},
    {OCR_SIZENESW, L"SizeNESW"},
    {OCR_SIZEWE, L"SizeWE"},
    {OCR_SIZENS, L"SizeNS"},
    {OCR_SIZEALL, L"SizeAll"},
    {OCR_ICOCUR, L"Help"}, // 8-P
    {OCR_NO, L"No"},
    {OCR_HAND, L"Hand"},
    {OCR_APPSTARTING, L"AppStarting"},
};
// clang-format on

// adapted from https://forum.juce.com/t/conversion-of-hicon-to-argb-image-icon2image/6565
bool CursorEmulator::ExtractImage(CursorInfo& cursor, HCURSOR instance, bool forceAlpha)
{
    cursor.w     = 0;
    cursor.h     = 0;
    cursor.image = NULL;
    BITMAP   bm;
    ICONINFO iconInfo;
    if(!GetIconInfo(instance, &iconInfo))
    {
        return false;
    }
    auto isMono = !iconInfo.hbmColor;
    if(isMono)
    {
        // monochrome icon
        if(!GetObject(iconInfo.hbmMask, sizeof(BITMAP), &bm))
        {
            return false;
        }
        cursor.w = bm.bmWidth;
        cursor.h = bm.bmHeight / 2;
    }
    else
    {
        if(!GetObject(iconInfo.hbmColor, sizeof(BITMAP), &bm))
        {
            return false;
        }
        cursor.w = bm.bmWidth;
        cursor.h = bm.bmHeight;
    }
    cursor.hotSpotX = iconInfo.xHotspot;
    cursor.hotSpotY = iconInfo.yHotspot;

    int        size = bm.bmWidth * bm.bmHeight * 4;
    BITMAPINFO infoheader;
    infoheader.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
    infoheader.bmiHeader.biWidth       = bm.bmWidth;
    infoheader.bmiHeader.biHeight      = bm.bmHeight;
    infoheader.bmiHeader.biPlanes      = 1;
    infoheader.bmiHeader.biBitCount    = 32;
    infoheader.bmiHeader.biCompression = BI_RGB;
    infoheader.bmiHeader.biSizeImage   = 0;
    // allocate Memory for Icon RGB data plus Icon mask plus ARGB buffer for the resulting image
    uint8_t* pMemBlock     = (uint8_t*)malloc(size * 2);
    uint8_t* pixelsIconRGB = (uint8_t*)pMemBlock;
    uint8_t* alphaPixels   = (uint8_t*)(pixelsIconRGB + size);

    HDC hDC = CreateCompatibleDC(NULL);
    if(!hDC)
    {
        return false;
    }

    if(isMono)
    {
        // get both masks into memory block
        HBITMAP hBmpOld = (HBITMAP)SelectObject(hDC, (HGDIOBJ)iconInfo.hbmMask);
        if(!GetDIBits(hDC, iconInfo.hbmMask, 0, bm.bmHeight, (LPVOID)pixelsIconRGB, &infoheader, DIB_RGB_COLORS))
        {
            free(pMemBlock);
            DeleteDC(hDC);
            return false;
        }
    }
    else
    {
        // Get Icon RGB data
        HBITMAP hBmpOld = (HBITMAP)SelectObject(hDC, (HGDIOBJ)iconInfo.hbmColor);
        if(!GetDIBits(hDC, iconInfo.hbmColor, 0, bm.bmHeight, (LPVOID)pixelsIconRGB, &infoheader, DIB_RGB_COLORS))
        {
            free(pMemBlock);
            DeleteDC(hDC);
            return false;
        }
        SelectObject(hDC, hBmpOld);
        // now get the mask
        if(!GetDIBits(hDC, iconInfo.hbmMask, 0, bm.bmHeight, (LPVOID)alphaPixels, &infoheader, DIB_RGB_COLORS))
        {
            free(pMemBlock);
            DeleteDC(hDC);
            return false;
        }
    }

    cursor.image       = (uint32_t*)malloc(cursor.h * cursor.w * 4);
    int x              = 0;
    int currentSrcPos  = 0;
    int currentDestPos = 0;
    int linePosSrc     = 0;
    int linePosDest    = 0;
    int lsSrc          = cursor.w * 4;
    int vsDest         = cursor.h - 1;
    for(int y = 0; y < cursor.h; y++)
    {
        if(isMono)
        {
            linePosSrc  = (vsDest - y) * lsSrc;
            linePosDest = y * cursor.w;
            for(x = 0; x < cursor.w; x++)
            {
                //pixels from Icon are stored in BGR vertical and horizontal flipped order
                currentDestPos = linePosDest + x;
                currentSrcPos  = linePosSrc + x * 4;
                auto andMask   = pixelsIconRGB[currentSrcPos];
                auto xorMask   = (pixelsIconRGB + (size / 2))[currentSrcPos];
                if(andMask)
                {
                    cursor.image[currentDestPos] = 0xffffffff;
                }
                else
                {
                    if(xorMask)
                    {
                        cursor.image[currentDestPos] = 0;
                    }
                    else
                    {
                        cursor.image[currentDestPos] = 0xff000000;
                    }
                }
            }
        }
        else
        {
            linePosSrc  = (vsDest - y) * lsSrc;
            linePosDest = y * cursor.w;
            for(x = 0; x < cursor.w; x++)
            {
                //pixels from Icon are stored in BGR vertical and horizontal flipped order
                currentDestPos = linePosDest + x;
                currentSrcPos  = linePosSrc + x * 4;
                cursor.image[currentDestPos] =
                    (((uint32_t)((((pixelsIconRGB[currentSrcPos + 2] << 16 /*Red*/) | (pixelsIconRGB[currentSrcPos + 1] << 8 /*Green*/)) | pixelsIconRGB[currentSrcPos] /*Blue*/) |
                                 ((alphaPixels[currentSrcPos] ? 0 : (forceAlpha ? 255 : pixelsIconRGB[currentSrcPos + 3])) << 24))) &
                     0xffffffffL);
            }
        }
    }
    free(pMemBlock);
    DeleteDC(hDC);
    return true;
}

CursorEmulator::CursorEmulator() : m_hidden {false} { }

CursorEmulator::~CursorEmulator()
{
    if(m_cursors.size())
    {
        ShowCursors(true);
        for(auto& c : m_cursors)
            if(c.image)
                free(c.image);
    }
}

void CursorEmulator::Init(winrt::com_ptr<ID3D11Device> device, HINSTANCE instance, HWND window)
{
    if(m_cursors.size())
        return;

    auto dpiScale = GetDpiForWindow(window) / (float)USER_DEFAULT_SCREEN_DPI;
    m_baseSize    = (int)roundf(GetCursorBaseSize() * dpiScale);

    m_blankCursor = (HCURSOR)LoadImage(instance, MAKEINTRESOURCE(IDC_SG_BLANK), IMAGE_CURSOR, 0, 0, 0);
    m_ibeamCursor = (HCURSOR)LoadImage(instance, MAKEINTRESOURCE(IDC_SG_IBEAM), IMAGE_CURSOR, m_baseSize, m_baseSize, 0);
    m_crossCursor = (HCURSOR)LoadImage(instance, MAKEINTRESOURCE(IDC_SG_CROSS), IMAGE_CURSOR, m_baseSize, m_baseSize, 0);

    for(const auto& c : cursors)
    {
        InitCursor(c.first, c.second.c_str(), device);
    }
}

std::wstring CursorEmulator::GetCursorFilePath(int type) const
{
    auto keyName = cursors.find(type);
    if(keyName == cursors.end())
        return L"";

    HKEY         hKey;
    std::wstring pathStr;
    if(RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("Control Panel\\Cursors"), 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
    {
        wchar_t path[MAX_PATH + 1];
        DWORD   size = MAX_PATH * sizeof(wchar_t);
        if(RegGetValue(hKey, NULL, keyName->second.c_str(), RRF_RT_REG_SZ, NULL, path, &size) == ERROR_SUCCESS)
        {
            if(lstrlen(path) > 0)
                pathStr = path;
        }
        RegCloseKey(hKey);
    }

    return pathStr;
}

int CursorEmulator::GetCursorBaseSize() const
{
    HKEY         hKey;
    std::wstring pathStr;
    if(RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("Control Panel\\Cursors"), 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
    {
        DWORD dvalue = 0;
        DWORD size   = sizeof(DWORD);
        RegGetValue(hKey, NULL, L"CursorBaseSize", RRF_RT_REG_DWORD, NULL, &dvalue, &size);
        RegCloseKey(hKey);
        return (int)dvalue;
    }

    return 0;
}

void CursorEmulator::InitCursor(int type, const wchar_t* name, winrt::com_ptr<ID3D11Device> device)
{
    CursorInfo ci;
    ci.cursorType = type;
    ci.name       = name;

    HCURSOR imageBase  = NULL;
    bool    forceAlpha = false;

    // snap handle to currently set cursor so we can detect which cursor type is being set by other apps
    ci.cursor = LoadCursor(NULL, MAKEINTRESOURCE(type));

    // try load cursor if it's customized
    auto filePath = GetCursorFilePath(type);
    if(filePath.size())
    {
        // load from file so taht we pick up non-standard cursors
        ci.original = (HCURSOR)LoadImage(NULL, filePath.c_str(), IMAGE_CURSOR, m_baseSize, m_baseSize, LR_LOADFROMFILE);
        imageBase   = ci.original;
    }
    else if(type == OCR_IBEAM)
    {
        // don't use builtins as they rely on AND/XOR masks
        ci.original = CopyCursor(ci.cursor);
        imageBase   = m_ibeamCursor;
        forceAlpha  = true;
    }
    else if(type == OCR_CROSS)
    {
        // don't use builtins as they rely on AND/XOR masks
        ci.original = CopyCursor(ci.cursor);
        imageBase   = m_crossCursor;
        forceAlpha  = true;
    }
    else
    {
        // use built-in
        ci.original = CopyCursor(ci.cursor);
        imageBase   = ci.cursor;
    }

    if(ExtractImage(ci, imageBase, forceAlpha))
    {
        D3D11_TEXTURE2D_DESC desc2 = {};
        desc2.Usage                = D3D11_USAGE_DEFAULT;
        desc2.BindFlags            = D3D11_BIND_SHADER_RESOURCE;
        desc2.CPUAccessFlags       = 0;
        desc2.MiscFlags            = 0;
        desc2.Width                = ci.w;
        desc2.Height               = ci.h;
        desc2.ArraySize            = 1;
        desc2.MipLevels            = 1;
        desc2.SampleDesc.Count     = 1;
        desc2.SampleDesc.Quality   = 0;
        desc2.Format               = DXGI_FORMAT_B8G8R8A8_UNORM;

        D3D11_SUBRESOURCE_DATA sbd = {};
        sbd.pSysMem                = ci.image;
        sbd.SysMemPitch            = ci.w * 4;
        sbd.SysMemSlicePitch       = 0;

        HRESULT hr;
        hr = device->CreateTexture2D(&desc2, &sbd, ci.texture.put());
        assert(SUCCEEDED(hr));
        hr = device->CreateShaderResourceView(ci.texture.get(), nullptr, ci.view.put());
        assert(SUCCEEDED(hr));
    }

    m_cursors.emplace_back(ci);
}

void CursorEmulator::Start()
{
    if(!m_active)
    {
        m_active = true;
    }
}

void CursorEmulator::Stop()
{
    if(m_active)
    {
        ShowCursors();
        m_active = false;
    }
}

bool CursorEmulator::Hidden() const
{
    return m_active && m_hidden;
}

void CursorEmulator::HideCursors()
{
    if(m_active && !m_hidden)
    {
        for(auto& c : m_cursors)
            HideCursor(c);
        m_hidden = true;
    }
}

void CursorEmulator::ShowCursors(bool force)
{
    if(force || (m_active && m_hidden))
    {
        SystemParametersInfo(SPI_SETCURSORS, 0, nullptr, 0);
        m_hidden = false;
    }
}

const wchar_t* CursorEmulator::GetCursorName() const
{
    CURSORINFO ci {.cbSize = sizeof(CURSORINFO)};
    GetCursorInfo(&ci);

    for(auto& c : m_cursors)
    {
        if(c.cursor == ci.hCursor)
            return c.name;
    }

    return L"Unknown";
}

const CursorEmulator::CursorInfo* CursorEmulator::GetCursor()
{
    CURSORINFO ci {.cbSize = sizeof(CURSORINFO)};
    GetCursorInfo(&ci);

    for(auto& c : m_cursors)
    {
        if(c.cursor == ci.hCursor)
            return &c;
    }

    return nullptr;
}

void CursorEmulator::HideCursor(CursorInfo& ci)
{
    auto cp = CopyCursor(m_blankCursor);
    SetSystemCursor(cp, ci.cursorType);
}
