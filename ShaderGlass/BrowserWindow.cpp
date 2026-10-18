/*
ShaderGlass: shader effect overlay
Copyright (C) 2021-2025 mausimus (mausimus.net)
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0
*/

#include "pch.h"

#include "resource.h"
#include "BrowserWindow.h"

constexpr int WINDOW_WIDTH  = 450;
constexpr int WINDOW_HEIGHT = 723;
constexpr int CX_BITMAP     = 24;
constexpr int CY_BITMAP     = 24;
constexpr int NUM_BITMAPS   = 3;
constexpr int BUTTON_WIDTH  = 140;
constexpr int PANEL_HEIGHT  = 40;
constexpr int STATIC_WIDTH  = 80;
constexpr int STATIC_HEIGHT = 30;
constexpr int SEARCH_WIDTH  = 60;
constexpr int SEARCH_HEIGHT = 25;
constexpr int TRACK_WIDTH   = 150;
constexpr int TRACK_HEIGHT  = 30;
constexpr int MAX_NAME      = 20;
constexpr int MAX_VALUE     = 200;

BrowserWindow::BrowserWindow(CaptureManager& captureManager) :
    m_captureManager(captureManager), m_captureOptions(captureManager.m_options), m_title(), m_windowClass(), m_font(0), m_dpiScale(1.0f)
{ }

LRESULT CALLBACK BrowserWindow::WndProcProxy(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    BrowserWindow* app;
    if(msg == WM_CREATE)
    {
        app = (BrowserWindow*)(((LPCREATESTRUCT)lParam)->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)app);
    }
    else
    {
        app = (BrowserWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    }
    return app->WndProc(hWnd, msg, wParam, lParam);
}

ATOM BrowserWindow::MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc   = BrowserWindow::WndProcProxy;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = 0;
    wcex.hInstance     = hInstance;
    wcex.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SHADERGLASS));
    wcex.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetSysColorBrush(COLOR_3DFACE);
    wcex.lpszMenuName  = 0;
    wcex.lpszClassName = m_windowClass;
    wcex.hIconSm       = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL BrowserWindow::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    m_instance = hInstance;

    RECT rect;
    rect.left   = 0;
    rect.top    = 0;
    rect.right  = (LONG)(WINDOW_WIDTH);
    rect.bottom = (LONG)(WINDOW_HEIGHT);
    AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, true, WS_EX_WINDOWEDGE);

    HWND hWnd = CreateWindowW(m_windowClass,
                              m_title,
                              WS_OVERLAPPEDWINDOW | WS_EX_WINDOWEDGE,
                              CW_USEDEFAULT,
                              CW_USEDEFAULT,
                              rect.right - rect.left,
                              rect.bottom - rect.top,
                              m_shaderWindow,
                              nullptr,
                              hInstance,
                              this);

    if(!hWnd)
    {
        return FALSE;
    }

    m_dpi      = GetDpiForWindow(hWnd);
    m_dpiScale = m_dpi / (float)USER_DEFAULT_SCREEN_DPI;
    if(m_dpi != USER_DEFAULT_SCREEN_DPI)
    {
        rect.right *= m_dpiScale;
        rect.bottom *= m_dpiScale;
        SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOMOVE);
    }

    NONCLIENTMETRICS metrics = {};
    metrics.cbSize           = sizeof(metrics);
    SystemParametersInfoForDpi(SPI_GETNONCLIENTMETRICS, metrics.cbSize, &metrics, 0, m_dpi);
    m_font = CreateFontIndirect(&metrics.lfCaptionFont);

    m_mainWindow = hWnd;

    Build();

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    SetWindowPos(m_mainWindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    return TRUE;
}

void BrowserWindow::Resize()
{
    auto dpi = GetDpiForWindow(m_mainWindow);
    if(dpi != m_dpi)
    {
        m_dpi      = dpi;
        m_dpiScale = dpi / (float)USER_DEFAULT_SCREEN_DPI;

        // resize fonts
        NONCLIENTMETRICS metrics = {};
        metrics.cbSize           = sizeof(metrics);
        SystemParametersInfoForDpi(SPI_GETNONCLIENTMETRICS, metrics.cbSize, &metrics, 0, m_dpi);
        DeleteObject(m_font);
        m_font = CreateFontIndirect(&metrics.lfCaptionFont);

        SendMessage(m_addFavButton, WM_SETFONT, (WPARAM)m_font, MAKELPARAM(TRUE, 0));
        SendMessage(m_delFavButton, WM_SETFONT, (WPARAM)m_font, MAKELPARAM(TRUE, 0));
        SendMessage(m_paramsButton, WM_SETFONT, (WPARAM)m_font, MAKELPARAM(TRUE, 0));
        SendMessage(m_searchLabel, WM_SETFONT, (WPARAM)m_font, MAKELPARAM(TRUE, 0));
        SendMessage(m_searchBox, WM_SETFONT, (WPARAM)m_font, MAKELPARAM(TRUE, 0));
        SendMessage(m_clearButton, WM_SETFONT, (WPARAM)m_font, MAKELPARAM(TRUE, 0));
        SendMessage(m_pixelSizeLabel, WM_SETFONT, (WPARAM)m_font, MAKELPARAM(TRUE, 0));
        SendMessage(m_pixelSizeValue, WM_SETFONT, (WPARAM)m_font, MAKELPARAM(TRUE, 0));
    }

    RECT rcClient;
    GetClientRect(m_mainWindow, &rcClient);
    SetWindowPos(m_treeControl, NULL, 0, (LONG)(SEARCH_HEIGHT * 1 * m_dpiScale), rcClient.right, rcClient.bottom - (LONG)((PANEL_HEIGHT * 2 + SEARCH_HEIGHT) * m_dpiScale), 0);
    SetWindowPos(m_resultsControl, NULL, 0, (LONG)(SEARCH_HEIGHT * 1 * m_dpiScale), rcClient.right, rcClient.bottom - (LONG)((PANEL_HEIGHT * 2 + SEARCH_HEIGHT) * m_dpiScale), 0);
    SetWindowPos(m_pixelSizeTrackBar,
                 NULL,
                 (LONG)(m_dpiScale * STATIC_WIDTH),
                 (LONG)(rcClient.bottom - (PANEL_HEIGHT * 1.75f * m_dpiScale)),
                 (LONG)(m_dpiScale * TRACK_WIDTH),
                 (LONG)(m_dpiScale * TRACK_HEIGHT),
                 0);
    SetWindowPos(m_searchLabel, NULL, 0, (LONG)(m_dpiScale * 2), (LONG)(m_dpiScale * SEARCH_WIDTH), (LONG)(m_dpiScale * (SEARCH_HEIGHT - 2)), 0);
    SetWindowPos(m_searchBox, NULL, (LONG)(m_dpiScale * SEARCH_WIDTH), 0, (LONG)(rcClient.right - m_dpiScale * 2 * SEARCH_WIDTH), (LONG)(m_dpiScale * SEARCH_HEIGHT), 0);
    SetWindowPos(m_clearButton, NULL, (LONG)(rcClient.right - m_dpiScale * SEARCH_WIDTH), 0, (LONG)(m_dpiScale * SEARCH_WIDTH), (LONG)(m_dpiScale * SEARCH_HEIGHT), 0);
    SetWindowPos(
        m_pixelSizeLabel, NULL, 2, (LONG)(rcClient.bottom - (PANEL_HEIGHT * 1.75f * m_dpiScale)), (LONG)(m_dpiScale * STATIC_WIDTH - 2), (LONG)(m_dpiScale * STATIC_HEIGHT), 0);
    SetWindowPos(m_pixelSizeValue,
                 NULL,
                 (LONG)((STATIC_WIDTH + TRACK_WIDTH) * m_dpiScale),
                 (LONG)(rcClient.bottom - (PANEL_HEIGHT * 1.75f * m_dpiScale)),
                 (LONG)(m_dpiScale * STATIC_WIDTH * 2),
                 (LONG)(m_dpiScale * STATIC_HEIGHT),
                 0);

    SetWindowPos(m_addFavButton, NULL, 0, rcClient.bottom - (LONG)(PANEL_HEIGHT * m_dpiScale), (LONG)(BUTTON_WIDTH * m_dpiScale), (LONG)(PANEL_HEIGHT * m_dpiScale), 0);
    SetWindowPos(m_delFavButton,
                 NULL,
                 (LONG)(BUTTON_WIDTH * m_dpiScale),
                 rcClient.bottom - (LONG)(PANEL_HEIGHT * m_dpiScale),
                 (LONG)(BUTTON_WIDTH * m_dpiScale),
                 (LONG)(PANEL_HEIGHT * m_dpiScale),
                 0);

    SetWindowPos(m_paramsButton,
                 NULL,
                 (LONG)(BUTTON_WIDTH * 2 * m_dpiScale),
                 rcClient.bottom - (LONG)(PANEL_HEIGHT * m_dpiScale),
                 rcClient.right - (LONG)(BUTTON_WIDTH * 2 * m_dpiScale),
                 (LONG)(PANEL_HEIGHT * m_dpiScale),
                 0);
}

BOOL BrowserWindow::CreateImageList()
{
    HBITMAP hbmp;

    if((m_imageList = ImageList_Create(CX_BITMAP, CY_BITMAP, ILC_COLOR32, NUM_BITMAPS, 0)) == NULL)
        return FALSE;

    hbmp    = LoadBitmap(m_instance, MAKEINTRESOURCE(IDB_FOLDER));
    g_nOpen = ImageList_Add(m_imageList, hbmp, (HBITMAP)NULL);
    DeleteObject(hbmp);

    hbmp      = LoadBitmap(m_instance, MAKEINTRESOURCE(IDB_FOLDER));
    g_nClosed = ImageList_Add(m_imageList, hbmp, (HBITMAP)NULL);
    DeleteObject(hbmp);

    hbmp        = LoadBitmap(m_instance, MAKEINTRESOURCE(IDB_SHADER));
    g_nDocument = ImageList_Add(m_imageList, hbmp, (HBITMAP)NULL);
    DeleteObject(hbmp);

    if(ImageList_GetImageCount(m_imageList) < 3)
        return FALSE;

    return TRUE;
}

void BrowserWindow::CreatePixelSizeSlider(const RECT& rcClient)
{
    m_pixelSizeTrackBar = CreateWindowEx(0,
                                         TRACKBAR_CLASS,
                                         L"Trackbar Control",
                                         WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
                                         (LONG)(m_dpiScale * STATIC_WIDTH),
                                         (LONG)(rcClient.bottom - (PANEL_HEIGHT * 2 * m_dpiScale)),
                                         (LONG)(m_dpiScale * TRACK_WIDTH),
                                         (LONG)(m_dpiScale * TRACK_HEIGHT),
                                         m_mainWindow,
                                         0,
                                         m_instance,
                                         NULL);

    SendMessage(m_pixelSizeTrackBar,
                TBM_SETRANGE,
                (WPARAM)TRUE, // redraw flag
                (LPARAM)MAKELONG(0, pixelSizes.size() - 1));

    SendMessage(m_pixelSizeTrackBar,
                TBM_SETPOS,
                (WPARAM)TRUE, // redraw flag
                (LPARAM)4);

    SendMessage(m_pixelSizeTrackBar, WM_SETFONT, (LPARAM)m_font, true);

    m_pixelSizeLabel = CreateWindowEx(0,
                                      L"STATIC",
                                      L"Pixel Size",
                                      SS_RIGHT | SS_NOTIFY | WS_CHILD | WS_VISIBLE,
                                      2,
                                      0,
                                      (LONG)(m_dpiScale * STATIC_WIDTH - 2),
                                      (LONG)(m_dpiScale * STATIC_HEIGHT),
                                      m_mainWindow,
                                      NULL,
                                      m_instance,
                                      NULL);
    SendMessage(m_pixelSizeLabel, WM_SETFONT, (LPARAM)m_font, true);

    const wchar_t* label = L"?";
    for(const auto& ps : pixelSizes)
    {
        if(m_captureOptions.pixelWidth == ps.second.w && m_captureOptions.pixelHeight == ps.second.h)
        {
            label = ps.second.text;
            break;
        }
    }

    m_pixelSizeValue = CreateWindowEx(
        0, L"STATIC", label, SS_LEFT | WS_CHILD | WS_VISIBLE, 0, 0, (LONG)(m_dpiScale * STATIC_WIDTH), (LONG)(m_dpiScale * STATIC_HEIGHT), m_mainWindow, NULL, m_instance, NULL);
    SendMessage(m_pixelSizeValue, WM_SETFONT, (LPARAM)m_font, true);
}

void BrowserWindow::BeginTree()
{
    hPrev         = (HTREEITEM)TVI_FIRST;
    hPrevRootItem = NULL;
    hPrevLev2Item = NULL;
    hPrevLev3Item = NULL;
}

HTREEITEM BrowserWindow::AddItemToTree(HWND hwndTV, const char* text, LPARAM lParam, int nLevel)
{
    TVITEM         tvi;
    TVINSERTSTRUCT tvins;
    HTREEITEM      hti;
    LPTSTR         lpszItem = convertCharArrayToLPCWSTR(text);

    tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;

    // Set the text of the item.
    tvi.pszText    = lpszItem;
    tvi.cchTextMax = sizeof(tvi.pszText) / sizeof(tvi.pszText[0]);

    // Assume the item is not a parent item, so give it a
    // document image.
    if(nLevel == 1 && lParam != WM_SHADER(0))
    {
        tvi.iImage         = g_nClosed;
        tvi.iSelectedImage = g_nClosed;
    }
    else
    {
        tvi.iImage         = g_nDocument;
        tvi.iSelectedImage = g_nDocument;
    }

    // Save the heading level in the item's application-defined
    // data area.
    tvi.lParam         = lParam; //(LPARAM)nLevel;
    tvins.item         = tvi;
    tvins.hInsertAfter = hPrev;

    // Set the parent item based on the specified level.
    if(nLevel == 1)
        tvins.hParent = TVI_ROOT;
    else if(nLevel == 2)
        tvins.hParent = hPrevRootItem;
    else if(nLevel == 3)
        tvins.hParent = hPrevLev2Item;
    else if(nLevel == 4)
        tvins.hParent = hPrevLev3Item;

    // Add the item to the tree-view control.
    hPrev = (HTREEITEM)SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvins);

    if(hPrev == NULL)
        return NULL;

    // Save the handle to the item.
    if(nLevel == 1)
        hPrevRootItem = hPrev;
    else if(nLevel == 2)
        hPrevLev2Item = hPrev;
    else if(nLevel == 3)
        hPrevLev3Item = hPrev;

    // The new item is a child item. Give the parent item a
    // closed folder bitmap to indicate it now has child items.
    if(nLevel > 1)
    {
        hti                = TreeView_GetParent(hwndTV, hPrev);
        tvi.mask           = TVIF_IMAGE | TVIF_SELECTEDIMAGE;
        tvi.hItem          = hti;
        tvi.iImage         = g_nClosed;
        tvi.iSelectedImage = g_nClosed;
        TreeView_SetItem(hwndTV, &tvi);
    }
    free(lpszItem);
    return hPrev;
}

void BrowserWindow::Build()
{
    RECT rcClient; // dimensions of client area
    GetClientRect(m_mainWindow, &rcClient);
    m_treeControl = CreateWindowEx(WS_EX_STATICEDGE,
                                   WC_TREEVIEW,
                                   TEXT("Shader Tree"),
                                   WS_VISIBLE | WS_CHILD | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS /* | TVS_FULLROWSELECT*/,
                                   0,
                                   (LONG)(SEARCH_HEIGHT * 1 * m_dpiScale),
                                   rcClient.right,
                                   rcClient.bottom - (LONG)((PANEL_HEIGHT * 2 + SEARCH_HEIGHT) * m_dpiScale),
                                   m_mainWindow,
                                   NULL,
                                   m_instance,
                                   NULL);

    m_resultsControl = CreateWindowEx(WS_EX_STATICEDGE,
                                      WC_TREEVIEW,
                                      TEXT("Search Results"),
                                      WS_CHILD | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS /* | TVS_FULLROWSELECT*/,
                                      0,
                                      (LONG)(SEARCH_HEIGHT * 1 * m_dpiScale),
                                      rcClient.right,
                                      rcClient.bottom - (LONG)((PANEL_HEIGHT * 2 + SEARCH_HEIGHT) * m_dpiScale),
                                      m_mainWindow,
                                      NULL,
                                      m_instance,
                                      NULL);

    CreateImageList();
    TreeView_SetImageList(m_treeControl, m_imageList, TVSIL_NORMAL);
    TreeView_SetImageList(m_resultsControl, m_imageList, TVSIL_NORMAL);

    m_searchLabel = CreateWindowEx(0,
                                   L"STATIC",
                                   L"Search: ",
                                   SS_RIGHT | SS_NOTIFY | WS_CHILD | WS_VISIBLE,
                                   0,
                                   0,
                                   (LONG)(m_dpiScale * SEARCH_WIDTH),
                                   (LONG)(m_dpiScale * SEARCH_HEIGHT),
                                   m_mainWindow,
                                   NULL,
                                   m_instance,
                                   NULL);
    SendMessage(m_searchLabel, WM_SETFONT, (LPARAM)m_font, true);

    m_searchBox = CreateWindowEx(WS_EX_STATICEDGE,
                                 TEXT("EDIT"),
                                 TEXT(""),
                                 WS_CHILD | WS_VISIBLE,
                                 0,
                                 (LONG)(m_dpiScale * SEARCH_WIDTH),
                                 (LONG)(m_dpiScale * SEARCH_WIDTH * 4),
                                 (LONG)(m_dpiScale * SEARCH_HEIGHT),
                                 m_mainWindow,
                                 NULL,
                                 m_instance,
                                 NULL);
    SendMessage(m_searchBox, WM_SETFONT, (LPARAM)m_font, true);

    m_clearButton = CreateWindow(L"BUTTON",
                                 L"Clear",
                                 WS_TABSTOP | WS_VISIBLE | WS_CHILD,
                                 rcClient.right - (LONG)(m_dpiScale * SEARCH_WIDTH * 5),
                                 0,
                                 (LONG)(SEARCH_WIDTH * m_dpiScale),
                                 (LONG)(SEARCH_HEIGHT * m_dpiScale),
                                 m_mainWindow,
                                 NULL,
                                 (HINSTANCE)GetWindowLongPtr(m_mainWindow, GWLP_HINSTANCE),
                                 NULL);
    SendMessage(m_clearButton, WM_SETFONT, (LPARAM)m_font, true);

    BeginTree();

    auto noneItem = AddNoneItem(m_treeControl, m_items);

    m_personalItems = AddItemToTree(m_treeControl, "Personal Favorites", -1, 1);

    LoadPersonal();

    m_imported = AddItemToTree(m_treeControl, "Imported", -1, 1);

    AddItemToTree(m_treeControl, "Community Favorites", -1, 1);

    for(int fp = 0; fp < sizeof(favoritePresets) / sizeof(const char*); fp++)
    {
        auto p = m_captureManager.FindByName(favoritePresets[fp]);
        if(p != -1)
        {
            m_favorites[WM_SHADER(p)] = AddItemToTree(m_treeControl, favoritePresets[fp], WM_SHADER(p), 2);
        }
    }

    auto raItem = BuildTree(m_treeControl, m_items, "");
    TreeView_Expand(m_treeControl, raItem, TVE_EXPAND);

    if(m_captureOptions.presetNo)
    {
        auto favorite = m_favorites.find(WM_SHADER(m_captureOptions.presetNo));
        if(favorite != m_favorites.end())
        {
            SendMessage(m_treeControl, TVM_SELECTITEM, TVGN_CARET, (LPARAM)favorite->second);
            if(noneItem != nullptr)
                TreeView_SelectSetFirstVisible(m_treeControl, noneItem);
        }
        else
        {
            auto item = m_items.find(WM_SHADER(m_captureOptions.presetNo));
            if(item != m_items.end())
            {
                SendMessage(m_treeControl, TVM_SELECTITEM, TVGN_CARET, (LPARAM)item->second);
            }
        }
    }

    /// build other controls
    CreatePixelSizeSlider(rcClient);

    m_addFavButton = CreateWindow(L"BUTTON",
                                  L"Add Favorite",
                                  WS_TABSTOP | WS_VISIBLE | WS_CHILD,
                                  0,
                                  rcClient.bottom - (LONG)(PANEL_HEIGHT * m_dpiScale),
                                  (LONG)(BUTTON_WIDTH * m_dpiScale),
                                  (LONG)(PANEL_HEIGHT * m_dpiScale),
                                  m_mainWindow,
                                  NULL,
                                  (HINSTANCE)GetWindowLongPtr(m_mainWindow, GWLP_HINSTANCE),
                                  NULL);
    SendMessage(m_addFavButton, WM_SETFONT, (LPARAM)m_font, true);

    m_delFavButton = CreateWindow(L"BUTTON",
                                  L"Remove Favorite",
                                  WS_TABSTOP | WS_VISIBLE | WS_CHILD,
                                  (LONG)(BUTTON_WIDTH * m_dpiScale),
                                  rcClient.bottom - (LONG)(PANEL_HEIGHT * m_dpiScale),
                                  (LONG)(BUTTON_WIDTH * m_dpiScale),
                                  (LONG)(PANEL_HEIGHT * m_dpiScale),
                                  m_mainWindow,
                                  NULL,
                                  (HINSTANCE)GetWindowLongPtr(m_mainWindow, GWLP_HINSTANCE),
                                  NULL);
    SendMessage(m_delFavButton, WM_SETFONT, (LPARAM)m_font, true);

    m_paramsButton = CreateWindow(L"BUTTON",
                                  L"Parameters",
                                  WS_TABSTOP | WS_VISIBLE | WS_CHILD,
                                  (LONG)(BUTTON_WIDTH * 2 * m_dpiScale),
                                  rcClient.bottom - (LONG)(PANEL_HEIGHT * m_dpiScale),
                                  rcClient.right - (LONG)(BUTTON_WIDTH * 2 * m_dpiScale),
                                  (LONG)(PANEL_HEIGHT * m_dpiScale),
                                  m_mainWindow,
                                  NULL,
                                  (HINSTANCE)GetWindowLongPtr(m_mainWindow, GWLP_HINSTANCE),
                                  NULL);
    SendMessage(m_paramsButton, WM_SETFONT, (LPARAM)m_font, true);

    Resize();
}

static bool contains_ic(const std::string& search_in, const std::string& search_for)
{
    auto it = std::search(
        search_in.begin(), search_in.end(), search_for.begin(), search_for.end(), [](unsigned char ch1, unsigned char ch2) { return std::toupper(ch1) == std::toupper(ch2); });
    return (it != search_in.end());
}

HTREEITEM BrowserWindow::AddNoneItem(HWND ctrl, std::map<UINT, HTREEITEM>& items)
{
    int i = 0;
    for(const auto& sp : m_captureManager.Presets())
    {
        if(sp->Category == "general")
        {
            auto id       = WM_SHADER(i);
            auto noneItem = AddItemToTree(m_treeControl, sp->Name.c_str(), id, 1);
            items[id]     = noneItem;
            return noneItem;
        }
        i++;
    }
    return nullptr;
}

HTREEITEM BrowserWindow::BuildTree(HWND ctrl, std::map<UINT, HTREEITEM>& items, const std::string& filter)
{
    auto categoryComp = [](const std::string& c1, const std::string& c2) {
        if(c1 == c2)
            return false;
        if(c1.starts_with(c2))
            return true;
        if(c2.starts_with(c1))
            return false;
        return c1 < c2;
    };
    auto shaderComp = [](const std::string& c1, const std::string& c2) {
        std::string s1(c1.length(), ' ');
        std::string s2(c2.length(), ' ');
        auto        lower = [](char c) { return tolower(c); };
        std::transform(c1.begin(), c1.end(), s1.begin(), lower);
        std::transform(c2.begin(), c2.end(), s2.begin(), lower);
        return s1 < s2;
    };
    std::map<std::string, std::map<std::string, UINT, decltype(shaderComp)>, decltype(categoryComp)> categoryMenus;

    int i = 0;
    for(const auto& sp : m_captureManager.Presets())
    {
        if(sp->Category == "general")
        {
            i++;
            continue;
        }
        if(categoryMenus.find(sp->Category) == categoryMenus.end())
        {
            categoryMenus.insert(std::make_pair(sp->Category, std::map<std::string, UINT, decltype(shaderComp)>()));
        }
        auto& menu = categoryMenus.find(sp->Category)->second;
        if(filter.empty() || contains_ic(sp->Name, filter) || contains_ic(sp->Category, filter))
        {
            menu.insert(std::make_pair(sp->Name, WM_SHADER(i)));
        }
        i++;
    }

    auto raItem = AddItemToTree(ctrl, "RetroArch Library", -1, 1);

    std::string parentCategory("");
    int         level = 2;
    for(auto m : categoryMenus)
    {
        if(!m.second.size())
            continue;
        auto slash = m.first.find('/');
        if(slash != std::string::npos)
        {
            // has a parent category
            auto thisParent = m.first.substr(0, slash);
            if(thisParent != parentCategory)
            {
                // add new parent
                parentCategory = thisParent;
                AddItemToTree(ctrl, parentCategory.c_str(), -1, 2);
            }
            level = 3;
            AddItemToTree(ctrl, m.first.substr(slash + 1).c_str(), -1, level);
        }
        else if(m.first == parentCategory)
        {
            // loose presents in this category
            level = 2;
        }
        else
        {
            // back to root
            if(parentCategory.size())
            {
                parentCategory = "";
            }
            level = 2;
            AddItemToTree(ctrl, m.first.c_str(), -1, level);
        }
        for(auto p : m.second)
        {
            auto item       = AddItemToTree(ctrl, p.first.c_str(), p.second, level + 1);
            items[p.second] = item;
        }
    }

    if(filter.size())
    {
        ExpandAll(ctrl, raItem);
    }

    return raItem;
}

void BrowserWindow::LoadPersonal()
{
    HKEY  hkey;
    DWORD dwDisposition;
    if(RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("Software\\ShaderGlass\\Personal"), 0, NULL, 0, KEY_READ | KEY_QUERY_VALUE, NULL, &hkey, &dwDisposition) == ERROR_SUCCESS)
    {
        DWORD   index = 0;
        auto    done  = false;
        wchar_t name[MAX_NAME];
        wchar_t value[MAX_VALUE];
        do
        {
            DWORD nameSize  = MAX_NAME;
            DWORD type      = REG_SZ;
            DWORD valueSize = MAX_VALUE * sizeof(wchar_t);
            auto  result    = RegEnumValue(hkey, index++, name, &nameSize, NULL, &type, (BYTE*)value, &valueSize);
            if(result == ERROR_SUCCESS)
            {
                try
                {
                    auto valueString = std::wstring(value);
                    auto separator   = valueString.find(':');
                    if(separator == std::wstring::npos)
                        continue;
                    auto categoryWName = valueString.substr(0, separator);
                    auto profileWName  = valueString.substr(separator + 1);
                    if(categoryWName.size() == 0 || profileWName.size() == 0 || categoryWName.size() >= MAX_VALUE || profileWName.size() >= MAX_VALUE)
                        continue;

                    char   categoryName[MAX_VALUE + 1];
                    char   profileName[MAX_VALUE + 1];
                    size_t categoryLen;
                    size_t profileLen;
                    wcstombs_s(&categoryLen, categoryName, categoryWName.data(), MAX_VALUE);
                    wcstombs_s(&profileLen, profileName, profileWName.data(), MAX_VALUE);
                    categoryName[categoryLen] = 0;
                    profileName[profileLen]   = 0;
                    for(int p = 0; p < m_captureManager.Presets().size(); p++)
                    {
                        const auto& preset = m_captureManager.Presets().at(p);
                        if(_strnicmp(preset->Category.c_str(), categoryName, MAX_VALUE) == 0 && _strnicmp(preset->Name.c_str(), profileName, MAX_VALUE) == 0)
                        {
                            auto id = WM_SHADER(p);
                            if(m_personal.find(id) == m_personal.end())
                            {
                                m_personal[id] = AddItemToTree(m_treeControl, profileName, id, 2);
                            }
                            continue;
                        }
                    }
                }
                catch(...)
                { }
            }
            else //if(result == ERROR_NO_MORE_ITEMS)
            {
                done = true;
            }
        } while(!done);
        RegCloseKey(hkey);
    }
    TreeView_SortChildren(m_treeControl, m_personalItems, false);
    TreeView_Expand(m_treeControl, m_personalItems, TVE_EXPAND);
}

void BrowserWindow::Search(const std::string& filter)
{
    SendMessage(m_resultsControl, WM_SETREDRAW, FALSE, 0);
    TreeView_DeleteAllItems(m_resultsControl);
    if(filter.size())
    {
        std::map<UINT, HTREEITEM> items;
        BeginTree();
        BuildTree(m_resultsControl, items, filter);
        if(TreeView_GetCount(m_resultsControl) == 1)
            TreeView_DeleteAllItems(m_resultsControl);
        else
        {
            auto root = TreeView_GetRoot(m_resultsControl);
            if(root != NULL)
            {
                TreeView_EnsureVisible(m_resultsControl, root);
            }
        }
    }
    SendMessage(m_resultsControl, WM_SETREDRAW, TRUE, 0);
    RedrawWindow(m_resultsControl, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);
}

void BrowserWindow::ExpandAll(HWND ctrl, HTREEITEM item)
{
    TreeView_Expand(ctrl, item, TVE_EXPAND);
    auto child = TreeView_GetChild(ctrl, item);
    while(child)
    {
        ExpandAll(ctrl, child);
        child = TreeView_GetNextSibling(ctrl, child);
    }
}

void BrowserWindow::SavePersonal()
{
    HKEY  hkey;
    DWORD dwDisposition;
    if(RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("Software\\ShaderGlass"), 0, NULL, 0, KEY_WRITE | KEY_SET_VALUE, NULL, &hkey, &dwDisposition) == ERROR_SUCCESS)
    {
        RegDeleteKey(hkey, L"Personal");
    }

    if(RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("Software\\ShaderGlass\\Personal"), 0, NULL, 0, KEY_WRITE | KEY_SET_VALUE, NULL, &hkey, &dwDisposition) == ERROR_SUCCESS)
    {
        int index = 0;
        try
        {
            for(const auto& p : m_personal)
            {
                // update value
                const auto& profile = m_captureManager.Presets().at(p.first - WM_SHADER(0));

                wchar_t value[MAX_VALUE];
                _snwprintf_s(value, MAX_VALUE, L"%S:%S", profile->Category.c_str(), profile->Name.c_str());
                wchar_t name[MAX_NAME];
                _snwprintf_s(name, MAX_NAME, L"%d", index++);
                RegSetValueEx(hkey, name, 0, REG_SZ, (PBYTE)value, (DWORD)(wcslen(value) * sizeof(wchar_t)));
            }
        }
        catch(...)
        { }
        RegCloseKey(hkey);
    }
}

LRESULT CALLBACK BrowserWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
    case WM_NOTIFY: {
        LPNMHDR lpnmh = (LPNMHDR)lParam;
        if(lpnmh->code == TVN_SELCHANGED)
        {
            LPNMTREEVIEW pnmtv = (LPNMTREEVIEW)lParam;
            if(pnmtv->itemNew.lParam >= 0)
            {
                // switch shaders if different
                if(WM_SHADER(m_captureOptions.presetNo) != pnmtv->itemNew.lParam)
                {
                    PostMessage(m_shaderWindow, WM_COMMAND, pnmtv->itemNew.lParam, 0);
                }
            }

            auto selected = TreeView_GetSelection(m_treeControl);
            if(selected != NULL)
            {
                auto parent = TreeView_GetParent(m_treeControl, selected);
                if(parent == m_imported)
                {
                    EnableWindow(m_addFavButton, false);
                    EnableWindow(m_delFavButton, false);
                }
                else if(parent == m_personalItems)
                {
                    EnableWindow(m_addFavButton, false);
                    EnableWindow(m_delFavButton, true);
                }
                else
                {
                    EnableWindow(m_addFavButton, true);
                    EnableWindow(m_delFavButton, true);
                }
            }
        }
        else if(lpnmh->code == TVN_KEYDOWN)
        {
            LPNMTVKEYDOWN ptvkd = (LPNMTVKEYDOWN)lParam;
            if(ptvkd->wVKey == VK_ESCAPE)
            {
                ShowWindow(m_mainWindow, SW_HIDE);
            }
            return 1;
        }
        break;
    }
    case WM_CTLCOLORSTATIC: {
        HDC hdcStatic = (HDC)wParam;
        SetBkColor(hdcStatic, GetSysColor(COLOR_3DFACE));
        return (LRESULT)(HBRUSH)GetSysColorBrush(COLOR_3DFACE);
    }
    case WM_CLOSE: {
        ShowWindow(hWnd, SW_HIDE);
        return 0;
    }
    case WM_SIZE: {
        Resize();
        break;
    }
    case WM_KEYDOWN: {
        if(wParam == VK_ESCAPE)
        {
            ShowWindow(m_mainWindow, SW_HIDE);
            return 1;
        }
        break;
    }
    case WM_HSCROLL: {
        if(lParam != 0)
        {
            auto pos = SendMessage(m_pixelSizeTrackBar, TBM_GETPOS, 0, 0);
            PostMessage(m_shaderWindow, WM_COMMAND, WM_PIXEL_SIZE(pos), 0);
        }
        return 0;
    }
    case WM_COMMAND: {
        UINT cmdCode = HIWORD(wParam);
        switch(cmdCode)
        {
        case EN_SETFOCUS: {
            if(lParam == (LPARAM)m_searchBox)
                m_searching = true;
            break;
        }
        case EN_KILLFOCUS: {
            if(lParam == (LPARAM)m_searchBox)
                m_searching = false;
            break;
        }
        case EN_CHANGE:
            if(lParam == (LPARAM)m_searchBox)
            {
                wchar_t input[256];
                auto    len = GetWindowText(m_searchBox, input, 255);
                if(len > 0)
                {
                    ShowWindow(m_treeControl, SW_HIDE);
                    ShowWindow(m_resultsControl, SW_SHOWNOACTIVATE);
                    char utfName[256];
                    utfName[0] = 0;
                    if(len > 1)
                    {
                        WideCharToMultiByte(CP_UTF8, 0, input, -1, utfName, 255, NULL, NULL);
                    }
                    Search(utfName);
                }
                else
                {
                    ShowWindow(m_resultsControl, SW_HIDE);
                    ShowWindow(m_treeControl, SW_SHOWNOACTIVATE);
                    Search("");
                }
            }
            break;
        }
        UINT wmId = LOWORD(wParam);
        switch(wmId)
        {
        case WM_USER: {
            if(HIWORD(wParam)) // quick toggle
                return 0;
            // sync shader
            auto favorite = m_favorites.find((UINT)lParam);
            if(favorite != m_favorites.end())
            {
                // check if already selected
                auto selected = TreeView_GetSelection(m_treeControl);
                if(selected == favorite->second)
                {
                    // selected from fav - do nothing
                    return 0;
                }
            }
            auto personal = m_personal.find((UINT)lParam);
            if(personal != m_personal.end())
            {
                // check if already selected
                auto selected = TreeView_GetSelection(m_treeControl);
                if(selected == personal->second)
                {
                    // selected from personal - do nothing
                    return 0;
                }
            }
            auto item = m_items.find((UINT)lParam);
            if(item != m_items.end())
            {
                PostMessage(m_treeControl, TVM_SELECTITEM, TVGN_CARET, (LPARAM)item->second);
            }

            return 0;
        }
        case WM_USER + 1: {
            auto id = (UINT)WM_SHADER(lParam);

            if(m_items.contains(id)) // in-place update
                return 0;

            TVINSERTSTRUCT is;
            is.hParent             = m_imported;
            is.hInsertAfter        = TVI_LAST;
            is.item.mask           = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
            is.item.pszText        = convertCharArrayToLPCWSTR(m_captureManager.Presets().at(lParam)->Name.c_str());
            is.item.cchTextMax     = sizeof(is.item.pszText) / sizeof(is.item.pszText[0]);
            is.item.iImage         = g_nDocument;
            is.item.iSelectedImage = g_nDocument;
            is.item.lParam         = id;
            m_items[id]            = TreeView_InsertItem(m_treeControl, &is);
            return 0;
        }
        case WM_USER + 2: {
            auto newPos = (UINT)WM_PIXEL_SIZE(lParam);
            auto oldPos = SendMessage(m_pixelSizeTrackBar, TBM_GETPOS, 0, 0);
            if(newPos != oldPos)
            {
                SendMessage(m_pixelSizeTrackBar, TBM_SETPOS, (WPARAM)TRUE, lParam);
                const auto& label = pixelSizes.at(newPos).text;
                SetWindowText(m_pixelSizeValue, label);
            }
            return 0;
        };
        case IDM_UPDATE_PARAMS: {
            wchar_t paramsCaption[50];
            std::unordered_set<std::string> params;
            for(const auto& pt : m_captureManager.Params())
            {
                const auto& p = std::get<1>(pt);
                if(p->maxValue != p->minValue)
                    params.insert(p->name);
            }
            _snwprintf_s(paramsCaption, 50, _T("Parameters (%lu)"), params.size());
            SetWindowText(m_paramsButton, paramsCaption);
            return 0;
        }
        case BN_CLICKED: {
            if(lParam == (LPARAM)m_addFavButton)
            {
                auto selected = TreeView_GetSelection(m_treeControl);
                if(selected == nullptr)
                    return 0;
                TVITEM tvi;
                tvi.mask  = TVIF_PARAM;
                tvi.hItem = selected;
                TreeView_GetItem(m_treeControl, &tvi);
                const auto id = (UINT)tvi.lParam;
                if(m_personal.find(id) == m_personal.end())
                {
                    const auto& preset = m_captureManager.Presets().at(id - WM_SHADER(0));
                    if(preset->Category == "Imported")
                        return 0;

                    TVINSERTSTRUCT is;
                    is.hParent             = m_personalItems;
                    is.hInsertAfter        = TVI_LAST;
                    is.item.mask           = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
                    is.item.pszText        = convertCharArrayToLPCWSTR(preset->Name.c_str());
                    is.item.cchTextMax     = sizeof(is.item.pszText) / sizeof(is.item.pszText[0]);
                    is.item.iImage         = g_nDocument;
                    is.item.iSelectedImage = g_nDocument;
                    is.item.lParam         = id;
                    m_personal[id]         = TreeView_InsertItem(m_treeControl, &is);
                    TreeView_SortChildren(m_treeControl, m_personalItems, false);
                    TreeView_Expand(m_treeControl, m_personalItems, TVE_EXPAND);
                    SavePersonal();
                }
            }
            else if(lParam == (LPARAM)m_delFavButton)
            {
                auto selected = TreeView_GetSelection(m_treeControl);
                if(selected == nullptr)
                    return 0;
                TVITEM tvi;
                tvi.mask  = TVIF_PARAM;
                tvi.hItem = selected;
                TreeView_GetItem(m_treeControl, &tvi);
                const auto id           = (UINT)tvi.lParam;
                const auto personalItem = m_personal.find(id);
                if(personalItem != m_personal.end())
                {
                    TreeView_DeleteItem(m_treeControl, personalItem->second);
                    m_personal.erase(personalItem->first);
                    SavePersonal();
                }
            }
            else if(lParam == (LPARAM)m_paramsButton)
            {
                PostMessage(m_shaderWindow, WM_COMMAND, IDM_SHADER_PARAMETERS, 0);
            }
            else if(lParam == (LPARAM)m_clearButton)
            {
                SetWindowText(m_searchBox, L"");
            }
            return 0;
        }
        }
    }
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

bool BrowserWindow::Create(_In_ HINSTANCE hInstance, _In_ int nCmdShow, _In_ HWND shaderWindow, _In_ HWND paramsWindow)
{
    m_shaderWindow = shaderWindow;
    m_paramsWindow = paramsWindow;

    LoadStringW(hInstance, IDS_BROWSER_TITLE, m_title, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SHADERBROWSER, m_windowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    if(!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    return TRUE;
}
