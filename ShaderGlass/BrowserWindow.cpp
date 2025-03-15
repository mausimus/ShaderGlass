#include "pch.h"

#include "resource.h"
#include "BrowserWindow.h"

constexpr int WINDOW_WIDTH  = 400;
constexpr int WINDOW_HEIGHT = 600;
constexpr int CX_BITMAP     = 24;
constexpr int CY_BITMAP     = 24;
constexpr int NUM_BITMAPS   = 3;

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
    wcex.hbrBackground = (HBRUSH)GetSysColorBrush(COLOR_MENU);
    wcex.lpszMenuName  = 0;
    wcex.lpszClassName = m_windowClass;
    wcex.hIconSm       = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL BrowserWindow::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    m_instance = hInstance;

    m_dpiScale = GetDpiForSystem() / 96.0f;

    RECT rect;
    rect.left   = 0;
    rect.top    = 0;
    rect.right  = WINDOW_WIDTH * m_dpiScale;
    rect.bottom = WINDOW_HEIGHT * m_dpiScale;
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

    NONCLIENTMETRICS metrics = {};
    metrics.cbSize           = sizeof(metrics);
    SystemParametersInfo(SPI_GETNONCLIENTMETRICS, metrics.cbSize, &metrics, 0);

    m_font       = CreateFontIndirect(&metrics.lfCaptionFont);
    m_mainWindow = hWnd;

    Build();

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    SetWindowPos(m_mainWindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    return TRUE;
}

void BrowserWindow::Resize()
{
    RECT rcClient;
    GetClientRect(m_mainWindow, &rcClient);
    SetWindowPos(m_treeControl, NULL, 0, 0, rcClient.right, rcClient.bottom, 0);
}

BOOL BrowserWindow::CreateImageList(HWND hwndTV)
{
    HIMAGELIST himl;
    HBITMAP    hbmp;

    if((himl = ImageList_Create(CX_BITMAP, CY_BITMAP, ILC_COLOR32, NUM_BITMAPS, 0)) == NULL)
        return FALSE;

    hbmp    = LoadBitmap(m_instance, MAKEINTRESOURCE(IDB_FOLDER));
    g_nOpen = ImageList_Add(himl, hbmp, (HBITMAP)NULL);
    DeleteObject(hbmp);

    hbmp      = LoadBitmap(m_instance, MAKEINTRESOURCE(IDB_FOLDER));
    g_nClosed = ImageList_Add(himl, hbmp, (HBITMAP)NULL);
    DeleteObject(hbmp);

    hbmp        = LoadBitmap(m_instance, MAKEINTRESOURCE(IDB_SHADER));
    g_nDocument = ImageList_Add(himl, hbmp, (HBITMAP)NULL);
    DeleteObject(hbmp);

    if(ImageList_GetImageCount(himl) < 3)
        return FALSE;

    TreeView_SetImageList(hwndTV, himl, TVSIL_NORMAL);

    return TRUE;
}

HTREEITEM BrowserWindow::AddItemToTree(HWND hwndTV, LPTSTR lpszItem, LPARAM lParam, int nLevel)
{
    TVITEM           tvi;
    TVINSERTSTRUCT   tvins;
    static HTREEITEM hPrev         = (HTREEITEM)TVI_FIRST;
    static HTREEITEM hPrevRootItem = NULL;
    static HTREEITEM hPrevLev2Item = NULL;
    static HTREEITEM hPrevLev3Item = NULL;
    HTREEITEM        hti;

    tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;

    // Set the text of the item.
    tvi.pszText    = lpszItem;
    tvi.cchTextMax = sizeof(tvi.pszText) / sizeof(tvi.pszText[0]);

    // Assume the item is not a parent item, so give it a
    // document image.
    if(nLevel == 1)
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

    return hPrev;
}

void BrowserWindow::Build()
{
    RECT rcClient; // dimensions of client area
    GetClientRect(m_mainWindow, &rcClient);
    m_treeControl = CreateWindowEx(0,
                                   WC_TREEVIEW,
                                   TEXT("Shader Tree"),
                                   WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS /* | TVS_FULLROWSELECT*/,
                                   0,
                                   0,
                                   rcClient.right,
                                   rcClient.bottom,
                                   m_mainWindow,
                                   NULL,
                                   m_instance,
                                   NULL);

    CreateImageList(m_treeControl);

    auto categoryComp = [](const std::string& c1, const std::string& c2) {
        if(c1 == c2)
            return false;
        if(c1.starts_with(c2))
            return true;
        if(c2.starts_with(c1))
            return false;
        return c1 < c2;
    };
    std::map<std::string, std::vector<std::pair<const char*, UINT>>, decltype(categoryComp)> categoryMenus;

    HTREEITEM noneItem = nullptr;

    int i = 0;
    for(const auto& sp : m_captureManager.Presets())
    {
        if(sp->Category == "general")
        {
            auto id     = WM_SHADER(i++);
            noneItem    = AddItemToTree(m_treeControl, convertCharArrayToLPCWSTR(sp->Name.c_str()), id, 1);
            m_items[id] = noneItem;
            continue;
        }
        if(categoryMenus.find(sp->Category) == categoryMenus.end())
        {
            categoryMenus.insert(std::make_pair(sp->Category, std::vector<std::pair<const char*, UINT>>()));
        }
        auto& menu = categoryMenus.find(sp->Category)->second;
        menu.push_back(std::make_pair(sp->Name.c_str(), WM_SHADER(i++)));
    }

    AddItemToTree(m_treeControl, convertCharArrayToLPCWSTR("Favorites"), -1, 1);

    for(int fp = 0; fp < sizeof(favoritePresets) / sizeof(const char*); fp++)
    {
        auto p = m_captureManager.FindByName(favoritePresets[fp]);
        if(p != -1)
        {
            m_favorites[WM_SHADER(p)] = AddItemToTree(m_treeControl, convertCharArrayToLPCWSTR(favoritePresets[fp]), WM_SHADER(p), 2);
        }
    }

    m_imported = AddItemToTree(m_treeControl, convertCharArrayToLPCWSTR("Imported"), -1, 1);

    auto raItem = AddItemToTree(m_treeControl, convertCharArrayToLPCWSTR("RetroArch Library"), -1, 1);

    std::string parentCategory("");
    int         level = 2;
    for(auto m : categoryMenus)
    {
        auto slash = m.first.find('/');
        if(slash != std::string::npos)
        {
            // has a parent category
            auto thisParent = m.first.substr(0, slash);
            if(thisParent != parentCategory)
            {
                // add new parent
                parentCategory = thisParent;
                AddItemToTree(m_treeControl, convertCharArrayToLPCWSTR(parentCategory.c_str()), -1, 2);
            }
            level = 3;
            AddItemToTree(m_treeControl, convertCharArrayToLPCWSTR(m.first.substr(slash + 1).c_str()), -1, level);
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
            AddItemToTree(m_treeControl, convertCharArrayToLPCWSTR(m.first.c_str()), -1, level);
        }
        for(auto p : m.second)
        {
            auto item         = AddItemToTree(m_treeControl, convertCharArrayToLPCWSTR(p.first), p.second, level + 1);
            m_items[p.second] = item;
        }
    }

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

    Resize();
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
    case WM_COMMAND: {
        UINT wmId = LOWORD(wParam);
        switch(wmId)
        {
        case WM_USER: {
            if(HIWORD(wParam)) // quick toggle
                return 0;
            // sync shader
            auto favorite = m_favorites.find(lParam);
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
            auto item = m_items.find(lParam);
            if(item != m_items.end())
            {
                PostMessage(m_treeControl, TVM_SELECTITEM, TVGN_CARET, (LPARAM)item->second);
            }

            return 0;
        }
        case WM_USER + 1: {
            int            id = WM_SHADER(lParam);

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
