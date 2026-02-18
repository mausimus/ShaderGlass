#pragma once

class CaptureSession;

class CaptureLib
{
public:
    static void Disable();
    static bool Load();

    CaptureLib(CaptureSession& session);
    void                            Start(winrt::com_ptr<ID3D11Device> device, bool window, bool cursor);
    void                            OnFrameArrived(void* data, UINT width, UINT height, UINT pitch);
    std::unique_lock<std::mutex>    Lock();
    winrt::com_ptr<ID3D11Texture2D> GetInputFrame();
    bool                            Active() const;
    void                            Stop();

private:
    typedef void(__stdcall* CAPTURE_CALLBACK_FUNC)(void* data, UINT width, UINT height, UINT pitch, void* context);
    typedef UINT(__stdcall* CaptureLibVersionFunc)();
    typedef HRESULT(__stdcall* CaptureLibInitFunc)();
    typedef HRESULT(__stdcall* CaptureLibStartFunc)(UINT, UINT, CAPTURE_CALLBACK_FUNC, void*);
    typedef HRESULT(__stdcall* CaptureLibStopFunc)();

    static bool                  Enabled;
    static HMODULE               CaptureLibModule;
    static CaptureLibVersionFunc CaptureLibVersion;
    static CaptureLibInitFunc    CaptureLibInit;
    static CaptureLibStartFunc   CaptureLibStart;
    static CaptureLibStopFunc    CaptureLibStop;
    static const UINT            CaptureLibExpectedVersion = 1;
    static const UINT            CaptureTypeDesktop        = 0;
    static const UINT            CaptureTypeWindow         = 1;

    std::mutex                          m_mutex;
    winrt::com_ptr<ID3D11Texture2D>     m_inputFrame;
    winrt::com_ptr<ID3D11Device>        m_device;
    winrt::com_ptr<ID3D11DeviceContext> m_context;
    CaptureSession&                     m_session;
    volatile int                        m_width {0};
    volatile int                        m_height {0};
    volatile bool                       m_active {false};
};
