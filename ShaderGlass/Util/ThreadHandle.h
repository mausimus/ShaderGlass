/*
ShaderGlass: RAII wrapper for Windows thread handles
Copyright (C) 2021-2025 mausimus (mausimus.net)
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0
*/

#pragma once

#include <Windows.h>
#include <stdexcept>

// RAII wrapper for Windows thread handles with automatic cleanup
class ThreadHandle
{
private:
    HANDLE m_handle;
    bool m_ownsHandle;

public:
    ThreadHandle() : m_handle(NULL), m_ownsHandle(false) {}

    explicit ThreadHandle(HANDLE handle) : m_handle(handle), m_ownsHandle(true)
    {
        if(m_handle == NULL)
        {
            throw std::runtime_error("Failed to create thread");
        }
    }

    // Create thread with proper error handling
    static ThreadHandle Create(LPTHREAD_START_ROUTINE startFunc, LPVOID param, DWORD creationFlags = 0)
    {
        DWORD threadId;
        HANDLE handle = CreateThread(NULL, 0, startFunc, param, creationFlags, &threadId);
        if(handle == NULL)
        {
            throw std::runtime_error("CreateThread failed with error: " + std::to_string(GetLastError()));
        }
        return ThreadHandle(handle);
    }

    ~ThreadHandle()
    {
        if(m_ownsHandle && m_handle != NULL)
        {
            CloseHandle(m_handle);
            m_handle = NULL;
        }
    }

    // Wait for thread to complete with timeout
    bool Join(DWORD timeoutMs = 5000)
    {
        if(m_handle == NULL)
            return true;

        DWORD result = WaitForSingleObject(m_handle, timeoutMs);
        if(result == WAIT_OBJECT_0)
        {
            return true; // Thread completed successfully
        }
        else if(result == WAIT_TIMEOUT)
        {
            // Thread didn't complete in time - this is dangerous but we'll log it
            // In production, you might want to TerminateThread as last resort
            return false;
        }
        else
        {
            // Wait failed
            return false;
        }
    }

    // Force terminate thread (use only as last resort after timeout)
    void Terminate(DWORD exitCode = 0)
    {
        if(m_handle != NULL)
        {
            TerminateThread(m_handle, exitCode);
        }
    }

    // Get raw handle (use sparingly)
    HANDLE get() const { return m_handle; }

    // Check if valid
    bool isValid() const { return m_handle != NULL; }

    // Release ownership without closing
    HANDLE release()
    {
        HANDLE temp = m_handle;
        m_handle = NULL;
        m_ownsHandle = false;
        return temp;
    }

    // No copying
    ThreadHandle(const ThreadHandle&) = delete;
    ThreadHandle& operator=(const ThreadHandle&) = delete;

    // Move semantics
    ThreadHandle(ThreadHandle&& other) noexcept : m_handle(other.m_handle), m_ownsHandle(other.m_ownsHandle)
    {
        other.m_handle = NULL;
        other.m_ownsHandle = false;
    }

    ThreadHandle& operator=(ThreadHandle&& other) noexcept
    {
        if(this != &other)
        {
            if(m_ownsHandle && m_handle != NULL)
            {
                CloseHandle(m_handle);
            }
            m_handle = other.m_handle;
            m_ownsHandle = other.m_ownsHandle;
            other.m_handle = NULL;
            other.m_ownsHandle = false;
        }
        return *this;
    }
};

// RAII wrapper for Windows event handles
class EventHandle
{
private:
    HANDLE m_handle;

public:
    EventHandle() : m_handle(NULL) {}

    explicit EventHandle(BOOL manualReset, BOOL initialState)
    {
        m_handle = CreateEvent(NULL, manualReset, initialState, NULL);
        if(m_handle == NULL)
        {
            throw std::runtime_error("CreateEvent failed");
        }
    }

    ~EventHandle()
    {
        if(m_handle != NULL)
        {
            CloseHandle(m_handle);
            m_handle = NULL;
        }
    }

    HANDLE get() const { return m_handle; }
    bool isValid() const { return m_handle != NULL; }

    void Set()
    {
        if(m_handle != NULL)
            SetEvent(m_handle);
    }

    void Reset()
    {
        if(m_handle != NULL)
            ResetEvent(m_handle);
    }

    // No copying
    EventHandle(const EventHandle&) = delete;
    EventHandle& operator=(const EventHandle&) = delete;
};
