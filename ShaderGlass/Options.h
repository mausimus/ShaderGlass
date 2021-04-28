#pragma once

#define MAX_PIXEL_SIZES 20U
#define MAX_ASPECT_RATIOS 20U
#define MAX_SHADERS 1000U
#define MAX_FRAME_SKIPS 20U
#define MAX_OUTPUT_SCALES 20U
#define MAX_CAPTURE_WINDOWS 50U
#define MAX_CAPTURE_DISPLAYS 10U
#define HK_FULLSCREEN 1000

#define WM_PIXEL_SIZE(i) (static_cast<UINT>(WM_USER) + i)
#define WM_ASPECT_RATIO(i) (static_cast<UINT> WM_PIXEL_SIZE(MAX_PIXEL_SIZES) + i)
#define WM_SHADER(i) (static_cast<UINT> WM_ASPECT_RATIO(MAX_ASPECT_RATIOS) + i)
#define WM_FRAME_SKIP(i) (static_cast<UINT> WM_SHADER(MAX_SHADERS) + i)
#define WM_OUTPUT_SCALE(i) (static_cast<UINT> WM_FRAME_SKIP(MAX_FRAME_SKIPS) + i)
#define WM_CAPTURE_WINDOW(i) (static_cast<UINT> WM_OUTPUT_SCALE(MAX_OUTPUT_SCALES) + i)
#define WM_CAPTURE_DISPLAY(i) (static_cast<UINT> WM_CAPTURE_WINDOW(MAX_CAPTURE_WINDOWS) + i)

struct PixelSizeInfo
{
    float       w;
    float       h;
    const char* text;
    const char* mnemonic;

    PixelSizeInfo(float w, float h, const char* text, const char* mnemonic) : w(w), h(h), text(text), mnemonic(mnemonic) { }
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

struct OutputScaleInfo
{
    float       s;
    LPCWSTR     text;
    const char* mnemonic;

    OutputScaleInfo(float s, LPCWSTR text, const char* mnemonic) : s(s), text(text), mnemonic(mnemonic) { }
};

struct CaptureWindow
{
    CaptureWindow(HWND hwnd, const std::string& name) : hwnd {hwnd}, name {name} { }
    HWND        hwnd;
    std::string name;
};

struct CaptureDisplay
{
    CaptureDisplay(HMONITOR monitor, const std::string& name) : monitor {monitor}, name {name} { }
    HMONITOR    monitor;
    std::string name;
};

static const std::map<UINT, PixelSizeInfo> pixelSizes = {
    {WM_PIXEL_SIZE(0), PixelSizeInfo(1.0f, 1.0f, "x1", "1")},
    {WM_PIXEL_SIZE(1), PixelSizeInfo(2.0f, 2.0f, "x2", "2")},
    {WM_PIXEL_SIZE(2), PixelSizeInfo(3.0f, 3.0f, "x3", "3")},
    {WM_PIXEL_SIZE(3), PixelSizeInfo(4.0f, 4.0f, "x4", "4")},
    {WM_PIXEL_SIZE(4), PixelSizeInfo(4.5f, 4.5f, "x4.5 (240p -> 1080p)", "4.5")},
    {WM_PIXEL_SIZE(5), PixelSizeInfo(5.0f, 5.0f, "x5", "5")},
    {WM_PIXEL_SIZE(6), PixelSizeInfo(5.4f, 5.4f, "x5.4 (200p -> 1080p)", "5.4")},
    {WM_PIXEL_SIZE(7), PixelSizeInfo(6.0f, 6.0f, "x6 (240p -> 1440p)", "6")},
    {WM_PIXEL_SIZE(8), PixelSizeInfo(7.0f, 7.0f, "x7", "7")},
    {WM_PIXEL_SIZE(9), PixelSizeInfo(7.2f, 7.2f, "x7.2 (200p -> 1440p)", "7.2")},
    {WM_PIXEL_SIZE(10), PixelSizeInfo(8.0f, 8.0f, "x8", "8")},
};

static const std::map<UINT, AspectRatioInfo> aspectRatios = {
    {WM_ASPECT_RATIO(0), AspectRatioInfo(1.0f, L"None", "1")},
    {WM_ASPECT_RATIO(1), AspectRatioInfo(1.2f, L"x1.2 (DOS, NTSC)", "1.2")},
    {WM_ASPECT_RATIO(2), AspectRatioInfo(0.9375f, L"x0.94 (PAL)", "0.94")},
    {WM_ASPECT_RATIO(3), AspectRatioInfo(0.8f, L"x0.8 (NES)", "0.8")},
    {WM_ASPECT_RATIO(4), AspectRatioInfo(0.857143f, L"x0.86 (SNES)", "0.86")},
};

static const std::map<UINT, FrameSkipInfo> frameSkips = {
    {WM_FRAME_SKIP(0), FrameSkipInfo(0, L"none", "1")},
    {WM_FRAME_SKIP(1), FrameSkipInfo(1, L"/2", "2")},
    {WM_FRAME_SKIP(2), FrameSkipInfo(2, L"/3", "3")},
    {WM_FRAME_SKIP(3), FrameSkipInfo(3, L"/4", "4")},
    {WM_FRAME_SKIP(4), FrameSkipInfo(4, L"/5", "5")},
    {WM_FRAME_SKIP(5), FrameSkipInfo(5, L"/6", "6")},
};

static const std::map<UINT, OutputScaleInfo> outputScales = {{WM_OUTPUT_SCALE(0), OutputScaleInfo(1.0f, L"100%", "100")},
                                                             {WM_OUTPUT_SCALE(1), OutputScaleInfo(1.5f, L"150%", "150")},
                                                             {WM_OUTPUT_SCALE(2), OutputScaleInfo(2.0f, L"200%", "200")},
                                                             {WM_OUTPUT_SCALE(3), OutputScaleInfo(2.0f, L"250%", "250")},
                                                             {WM_OUTPUT_SCALE(4), OutputScaleInfo(3.0f, L"300%", "300")},
                                                             {WM_OUTPUT_SCALE(5), OutputScaleInfo(3.5f, L"350%", "350")},
                                                             {WM_OUTPUT_SCALE(6), OutputScaleInfo(4.0f, L"400%", "400")},
                                                             {WM_OUTPUT_SCALE(7), OutputScaleInfo(5.0f, L"500%", "500")},
                                                             {WM_OUTPUT_SCALE(8), OutputScaleInfo(6.0f, L"600%", "600")},
                                                             {WM_OUTPUT_SCALE(9), OutputScaleInfo(7.0f, L"700%", "700")},
                                                             {WM_OUTPUT_SCALE(10), OutputScaleInfo(8.0f, L"800%", "800")},
                                                             {WM_OUTPUT_SCALE(11), OutputScaleInfo(9.0f, L"900%", "900")},
                                                             {WM_OUTPUT_SCALE(12), OutputScaleInfo(10.0f, L"1000%", "1000")}};
