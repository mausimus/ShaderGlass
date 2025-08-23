/*
ShaderGlass: shader effect overlay
Copyright (C) 2021-2025 mausimus (mausimus.net)
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0
*/

#pragma once

#define MAX_PIXEL_SIZES 20U
#define MAX_ASPECT_RATIOS 20U
#define MAX_SHADERS 2100U
#define MAX_FRAME_SKIPS 20U
#define MAX_OUTPUT_SCALES 20U
#define MAX_CAPTURE_WINDOWS 100U
#define MAX_CAPTURE_DISPLAYS 10U
#define MAX_CAPTURE_DEVICE_FORMATS 1024U
#define MAX_RECENT_PROFILES 20U
#define MAX_RECENT_IMPORTS 20U
#define MAX_SUBFRAMES 10U
#define MAX_GPU 4U
#define MAX_GPUS 17U

#define WM_PIXEL_SIZE(i) (static_cast<UINT>(WM_USER) + i)
#define WM_ASPECT_RATIO(i) (static_cast<UINT> WM_PIXEL_SIZE(MAX_PIXEL_SIZES) + i)
#define WM_SHADER(i) (static_cast<UINT> WM_ASPECT_RATIO(MAX_ASPECT_RATIOS) + i)
#define WM_FRAME_SKIP(i) (static_cast<UINT> WM_SHADER(MAX_SHADERS) + i)
#define WM_OUTPUT_SCALE(i) (static_cast<UINT> WM_FRAME_SKIP(MAX_FRAME_SKIPS) + i)
#define WM_CAPTURE_WINDOW(i) (static_cast<UINT> WM_OUTPUT_SCALE(MAX_OUTPUT_SCALES) + i)
#define WM_CAPTURE_DISPLAY(i) (static_cast<UINT> WM_CAPTURE_WINDOW(MAX_CAPTURE_WINDOWS) + i)
#define WM_RECENT_PROFILE(i) (static_cast<UINT> WM_CAPTURE_DISPLAY(MAX_CAPTURE_DISPLAYS) + i)
#define WM_RECENT_IMPORT(i) (static_cast<UINT> WM_RECENT_PROFILE(MAX_RECENT_PROFILES) + i)
#define WM_CAPTURE_DEVICE_FORMAT(i) (static_cast<UINT> WM_RECENT_IMPORT(MAX_RECENT_IMPORTS) + i)
#define WM_SUBFRAMES(i) (static_cast<UINT> WM_CAPTURE_DEVICE_FORMAT(MAX_CAPTURE_DEVICE_FORMATS) + i)
#define WM_GPU(ci, ri) (static_cast<UINT> WM_SUBFRAMES(MAX_SUBFRAMES) + (ci << 2) + ri)

#define CUSTOM_MNEMONIC "Custom"

#define FORCE_FLIPMODE
#define FORCE_MAXCAPTURE

struct PixelSizeInfo
{
    float          w;
    float          h;
    const wchar_t* text;
    const char*    mnemonic;

    PixelSizeInfo(float w, float h, const wchar_t* text, const char* mnemonic) : w(w), h(h), text(text), mnemonic(mnemonic) { }
};

struct AspectRatioInfo
{
    float       r;
    LPCWSTR     text;
    const char* mnemonic;

    AspectRatioInfo(float r, LPCWSTR text, const char* mnemonic) : r(r), text(text), mnemonic(mnemonic) { }
};

struct FrameSkipInfo
{
    unsigned    s;
    LPCWSTR     text;
    const char* mnemonic;

    FrameSkipInfo(int s, LPCWSTR text, const char* mnemonic) : s(s), text(text), mnemonic(mnemonic) { }
};

struct SubFrameInfo
{
    unsigned    s;
    LPCWSTR     text;
    const char* mnemonic;

    SubFrameInfo(int s, LPCWSTR text, const char* mnemonic) : s(s), text(text), mnemonic(mnemonic) { }
};

struct OutputScaleInfo
{
    float       s;
    LPCWSTR     text;
    const char* mnemonic;

    OutputScaleInfo(float s, LPCWSTR text, const char* mnemonic) : s(s), text(text), mnemonic(mnemonic) { }
};

struct CaptureWindow
{
    CaptureWindow(HWND hwnd, const std::wstring& name) : hwnd {hwnd}, name {name} { }
    HWND         hwnd;
    std::wstring name;
};

struct CaptureDisplay
{
    CaptureDisplay(HMONITOR monitor, const std::string& name) : monitor {monitor}, name {name} { }
    HMONITOR    monitor;
    std::string name;
};

struct CaptureFormat
{
    unsigned     no;
    std::wstring name;
    std::string  id;

    unsigned bucket;
    unsigned wh;
    float    fps;
    int      priority;

    int deviceFormatNo;
};

struct CaptureDevice
{
    unsigned     no;
    std::wstring name;
    LPWSTR       symlink;

    std::vector<CaptureFormat> formats;
};

struct GraphicsAdapter
{
    unsigned                     no;
    std::wstring                 name;
    winrt::com_ptr<IDXGIAdapter> adapter;
    LUID                         luid;

    bool capture;
    bool render;
};

struct HotkeyInfo
{
    HotkeyInfo(UINT id, WORD defaultKey, const wchar_t* name, const wchar_t* accelerator) :
        id {id}, currentKey {defaultKey}, defaultKey {defaultKey}, name {name}, accelerator {accelerator}
    { }
    const wchar_t* name;
    UINT           id;
    WORD           currentKey;
    WORD           defaultKey;
    const wchar_t* accelerator;
};

static const std::map<UINT, PixelSizeInfo> pixelSizes = {{WM_PIXEL_SIZE(0), PixelSizeInfo(1.0f, 1.0f, L"x1", "1")},
                                                         {WM_PIXEL_SIZE(1), PixelSizeInfo(2.0f, 2.0f, L"x2", "2")},
                                                         {WM_PIXEL_SIZE(2), PixelSizeInfo(2.25f, 2.25f, L"x2.25 (480p -> 1080p)", "2.25")},
                                                         {WM_PIXEL_SIZE(3), PixelSizeInfo(3.0f, 3.0f, L"x3", "3")},
                                                         {WM_PIXEL_SIZE(4), PixelSizeInfo(4.0f, 4.0f, L"x4", "4")},
                                                         {WM_PIXEL_SIZE(5), PixelSizeInfo(4.5f, 4.5f, L"x4.5 (240p -> 1080p)", "4.5")},
                                                         {WM_PIXEL_SIZE(6), PixelSizeInfo(5.0f, 5.0f, L"x5", "5")},
                                                         {WM_PIXEL_SIZE(7), PixelSizeInfo(5.4f, 5.4f, L"x5.4 (200p -> 1080p)", "5.4")},
                                                         {WM_PIXEL_SIZE(8), PixelSizeInfo(6.0f, 6.0f, L"x6 (240p -> 1440p)", "6")},
                                                         {WM_PIXEL_SIZE(9), PixelSizeInfo(7.0f, 7.0f, L"x7", "7")},
                                                         {WM_PIXEL_SIZE(10), PixelSizeInfo(7.2f, 7.2f, L"x7.2 (200p -> 1440p)", "7.2")},
                                                         {WM_PIXEL_SIZE(11), PixelSizeInfo(8.0f, 8.0f, L"x8", "8")},
                                                         {WM_PIXEL_SIZE(12), PixelSizeInfo(9.0f, 9.0f, L"x9 (240p -> 4K)", "9")},
                                                         {WM_PIXEL_SIZE(13), PixelSizeInfo(10.0f, 10.0f, L"x10", "10")},
                                                         {WM_PIXEL_SIZE(14), PixelSizeInfo(10.8f, 10.8f, L"x10.8 (200p -> 4K)", "10.8")}};

static const std::map<UINT, SubFrameInfo> subFrames = {{WM_SUBFRAMES(0), SubFrameInfo(0, L"disabled", "0")},
                                                       {WM_SUBFRAMES(1), SubFrameInfo(2, L"2 (120 Hz)", "2")},
                                                       {WM_SUBFRAMES(2), SubFrameInfo(3, L"3 (180 Hz)", "3")},
                                                       {WM_SUBFRAMES(3), SubFrameInfo(4, L"4 (240 Hz)", "4")},
                                                       {WM_SUBFRAMES(4), SubFrameInfo(5, L"5 (300 Hz)", "5")},
                                                       {WM_SUBFRAMES(5), SubFrameInfo(6, L"6 (360 Hz)", "6")},
                                                       {WM_SUBFRAMES(6), SubFrameInfo(7, L"7 (420 Hz)", "7")},
                                                       {WM_SUBFRAMES(7), SubFrameInfo(8, L"8 (480 Hz)", "8")}};

static std::map<UINT, AspectRatioInfo> aspectRatios = {
    {WM_ASPECT_RATIO(0), AspectRatioInfo(1.0f, L"None", "1")},
    {WM_ASPECT_RATIO(1), AspectRatioInfo(1.2f, L"x1.2 (DOS, NTSC)", "1.2")},
    {WM_ASPECT_RATIO(2), AspectRatioInfo(0.9375f, L"x0.94 (PAL)", "0.94")},
    {WM_ASPECT_RATIO(3), AspectRatioInfo(0.8f, L"x0.8 (NES)", "0.8")},
    {WM_ASPECT_RATIO(4), AspectRatioInfo(0.857143f, L"x0.86 (SNES)", "0.86")},
    {WM_ASPECT_RATIO(5), AspectRatioInfo(0.5f, L"x0.5 (double wide)", "0.5")},
    {WM_ASPECT_RATIO(6), AspectRatioInfo(2.0f, L"x2.0 (double tall)", "2.0")},
    {WM_ASPECT_RATIO(7), AspectRatioInfo(1.0f, L"Custom", CUSTOM_MNEMONIC)},
};

static const std::map<UINT, FrameSkipInfo> frameSkips = {{WM_FRAME_SKIP(0), FrameSkipInfo(0, L"100% (VSync)", "1")},
                                                         {WM_FRAME_SKIP(1), FrameSkipInfo(1, L"1 / 2", "2")},
                                                         {WM_FRAME_SKIP(2), FrameSkipInfo(2, L"1 / 3", "3")},
                                                         {WM_FRAME_SKIP(3), FrameSkipInfo(3, L"1 / 4", "4")},
                                                         {WM_FRAME_SKIP(4), FrameSkipInfo(4, L"1 / 5", "5")},
                                                         {WM_FRAME_SKIP(5), FrameSkipInfo(5, L"1 / 6", "6")},
                                                         {WM_FRAME_SKIP(6), FrameSkipInfo(6, L"1 / 7", "7")},
                                                         {WM_FRAME_SKIP(7), FrameSkipInfo(7, L"1 / 8", "8")},
                                                         {WM_FRAME_SKIP(8), FrameSkipInfo(8, L"1 / 9", "9")},
                                                         {WM_FRAME_SKIP(9), FrameSkipInfo(9, L"1 / 10", "10")},
                                                         {WM_FRAME_SKIP(10), FrameSkipInfo(11, L"1 / 12", "12")},
                                                         {WM_FRAME_SKIP(11), FrameSkipInfo(13, L"1 / 14", "14")},
                                                         {WM_FRAME_SKIP(12), FrameSkipInfo(15, L"1 / 16", "16")},
                                                         {WM_FRAME_SKIP(13), FrameSkipInfo(17, L"1 / 18", "18")},
                                                         {WM_FRAME_SKIP(14), FrameSkipInfo(19, L"1 / 20", "20")}};

static const std::map<UINT, OutputScaleInfo> outputScales = {{WM_OUTPUT_SCALE(0), OutputScaleInfo(1.0f, L"100%", "100")},
                                                             {WM_OUTPUT_SCALE(1), OutputScaleInfo(1.5f, L"150%", "150")},
                                                             {WM_OUTPUT_SCALE(2), OutputScaleInfo(2.0f, L"200%", "200")},
                                                             {WM_OUTPUT_SCALE(3), OutputScaleInfo(2.5f, L"250%", "250")},
                                                             {WM_OUTPUT_SCALE(4), OutputScaleInfo(3.0f, L"300%", "300")},
                                                             {WM_OUTPUT_SCALE(5), OutputScaleInfo(3.5f, L"350%", "350")},
                                                             {WM_OUTPUT_SCALE(6), OutputScaleInfo(4.0f, L"400%", "400")},
                                                             {WM_OUTPUT_SCALE(7), OutputScaleInfo(5.0f, L"500%", "500")},
                                                             {WM_OUTPUT_SCALE(8), OutputScaleInfo(6.0f, L"600%", "600")},
                                                             {WM_OUTPUT_SCALE(9), OutputScaleInfo(7.0f, L"700%", "700")},
                                                             {WM_OUTPUT_SCALE(10), OutputScaleInfo(8.0f, L"800%", "800")},
                                                             {WM_OUTPUT_SCALE(11), OutputScaleInfo(9.0f, L"900%", "900")},
                                                             {WM_OUTPUT_SCALE(12), OutputScaleInfo(10.0f, L"1000%", "1000")}};

static const char* defaultPreset     = "crt-beam-simulator";
//static const char* favoritePresets[] = {};
