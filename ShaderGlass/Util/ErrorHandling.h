/*
ShaderGlass: Error handling utilities for DirectX and Windows API
Copyright (C) 2021-2025 mausimus (mausimus.net)
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0
*/

#pragma once

#include <Windows.h>
#include <comdef.h>
#include <stdexcept>
#include <string>
#include <sstream>

// Convert HRESULT to string description
inline std::string HResultToString(HRESULT hr)
{
    _com_error err(hr);
    LPCTSTR errMsg = err.ErrorMessage();

    std::wstring ws(errMsg);
    int len = WideCharToMultiByte(CP_UTF8, 0, ws.c_str(), (int)ws.size(), nullptr, 0, nullptr, nullptr);
    std::string str(len, '\0');
    WideCharToMultiByte(CP_UTF8, 0, ws.c_str(), (int)ws.size(), str.data(), len, nullptr, nullptr);

    std::stringstream ss;
    ss << "HRESULT 0x" << std::hex << hr << ": " << str;
    return ss.str();
}

// Throw exception if HRESULT indicates failure
// Use this to replace assert(SUCCEEDED(hr)) which is compiled out in Release builds
inline void ThrowIfFailed(HRESULT hr, const char* file, int line, const char* expression)
{
    if(FAILED(hr))
    {
        std::stringstream ss;
        ss << "DirectX Error at " << file << ":" << line << "\n"
           << "Expression: " << expression << "\n"
           << HResultToString(hr);
        throw std::runtime_error(ss.str());
    }
}

// Macro for checking HRESULT with source location
#define THROW_IF_FAILED(hr) ThrowIfFailed(hr, __FILE__, __LINE__, #hr)

// Alternative: Log and continue (for non-critical errors)
// Returns true if failed, false if succeeded
inline bool LogIfFailed(HRESULT hr, const char* file, int line, const char* expression)
{
    if(FAILED(hr))
    {
        // In production, this would write to a log file
        // For now, use OutputDebugString
        std::stringstream ss;
        ss << "DirectX Warning at " << file << ":" << line << "\n"
           << "Expression: " << expression << "\n"
           << HResultToString(hr) << "\n";

        OutputDebugStringA(ss.str().c_str());
        return true;
    }
    return false;
}

#define LOG_IF_FAILED(hr) LogIfFailed(hr, __FILE__, __LINE__, #hr)

// For compatibility with existing THROW macro (used in some places)
#ifndef THROW
#define THROW(hr) THROW_IF_FAILED(hr)
#endif
