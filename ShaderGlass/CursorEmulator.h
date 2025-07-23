/*
ShaderGlass: shader effect overlay
Copyright (C) 2021-2025 mausimus (mausimus.net)
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0
*/

#pragma once

class CursorEmulator
{
public:
    struct CursorInfo
    {
        int            cursorType;
        HCURSOR        cursor;
        HCURSOR        original;
        const wchar_t* name;
        uint32_t*      image;
        int            w;
        int            h;
        int            hotSpotX;
        int            hotSpotY;

        winrt::com_ptr<ID3D11Texture2D>          texture;
        winrt::com_ptr<ID3D11ShaderResourceView> view;
    };

    CursorEmulator();

    void Init(winrt::com_ptr<ID3D11Device> device, HINSTANCE instance, HWND window);

    void                              Start();
    void                              Stop();
    void                              HideCursors();
    void                              ShowCursors(bool force = false);
    bool                              Hidden() const;
    const wchar_t*                    GetCursorName() const;
    const CursorEmulator::CursorInfo* GetCursor();

    ~CursorEmulator();

private:
    HCURSOR m_blankCursor;
    HCURSOR m_ibeamCursor;
    HCURSOR m_crossCursor;

    std::vector<CursorInfo> m_cursors;
    bool                    m_hidden {false};
    bool                    m_active {false};
    int                     m_baseSize {0};

    std::wstring GetCursorFilePath(int type) const;
    int          GetCursorBaseSize() const;

    void InitCursor(int type, const wchar_t* name, winrt::com_ptr<ID3D11Device> device);
    bool ExtractImage(CursorInfo& cursor, HCURSOR instance, bool forceAlpha);
    void HideCursor(CursorInfo& ci);
};