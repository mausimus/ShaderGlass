/*
ShaderGlass: shader effect overlay
Copyright (C) 2021-2025 mausimus (mausimus.net)
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0
*/

#pragma once

#include <string>
#include <fstream>
#include <mutex>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <filesystem>

namespace ShaderGlass {

enum class LogLevel
{
    TRACE,   // Detailed debug information
    DEBUG,   // Debug information
    INFO,    // General informational messages
    WARNING, // Warning messages
    ERROR,   // Error messages
    FATAL    // Fatal error messages
};

class Logger
{
public:
    static Logger& Instance()
    {
        static Logger instance;
        return instance;
    }

    // Configure logger
    void SetLevel(LogLevel level) { m_minLevel = level; }
    void SetFile(const std::string& filename)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if(m_file.is_open())
            m_file.close();

        // Create Logs directory if needed
        std::filesystem::create_directories("Logs");
        m_file.open(filename, std::ios::out | std::ios::app);

        if(m_file.is_open())
        {
            Info("Logger", "Log file opened: " + filename);
        }
    }
    void SetConsoleOutput(bool enabled) { m_consoleOutput = enabled; }
    void SetFileOutput(bool enabled) { m_fileOutput = enabled; }

    // Logging methods
    void Log(LogLevel level, const std::string& component, const std::string& message)
    {
        if(level < m_minLevel)
            return;

        std::lock_guard<std::mutex> lock(m_mutex);

        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

        std::stringstream ss;
        std::tm tm;
        localtime_s(&tm, &time);

        ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S")
           << "." << std::setfill('0') << std::setw(3) << ms.count()
           << " [" << LevelToString(level) << "] "
           << "[" << component << "] "
           << message;

        std::string logLine = ss.str();

        if(m_consoleOutput)
        {
            OutputDebugStringA((logLine + "\n").c_str());
        }

        if(m_fileOutput && m_file.is_open())
        {
            m_file << logLine << std::endl;
            m_file.flush();
        }
    }

    // Convenience methods
    void Trace(const std::string& component, const std::string& message) { Log(LogLevel::TRACE, component, message); }
    void Debug(const std::string& component, const std::string& message) { Log(LogLevel::DEBUG, component, message); }
    void Info(const std::string& component, const std::string& message) { Log(LogLevel::INFO, component, message); }
    void Warning(const std::string& component, const std::string& message) { Log(LogLevel::WARNING, component, message); }
    void Error(const std::string& component, const std::string& message) { Log(LogLevel::ERROR, component, message); }
    void Fatal(const std::string& component, const std::string& message) { Log(LogLevel::FATAL, component, message); }

    // Performance logging helpers
    class ScopedTimer
    {
    public:
        ScopedTimer(const std::string& component, const std::string& operation)
            : m_component(component), m_operation(operation), m_start(std::chrono::high_resolution_clock::now())
        {
        }

        ~ScopedTimer()
        {
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - m_start).count();

            std::stringstream ss;
            ss << m_operation << " took " << duration << " μs";
            Logger::Instance().Debug(m_component, ss.str());
        }

    private:
        std::string m_component;
        std::string m_operation;
        std::chrono::high_resolution_clock::time_point m_start;
    };

    ~Logger()
    {
        if(m_file.is_open())
        {
            m_file.close();
        }
    }

private:
    Logger() : m_minLevel(LogLevel::INFO), m_consoleOutput(true), m_fileOutput(false) {}
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    static const char* LevelToString(LogLevel level)
    {
        switch(level)
        {
        case LogLevel::TRACE:   return "TRACE";
        case LogLevel::DEBUG:   return "DEBUG";
        case LogLevel::INFO:    return "INFO ";
        case LogLevel::WARNING: return "WARN ";
        case LogLevel::ERROR:   return "ERROR";
        case LogLevel::FATAL:   return "FATAL";
        default:                return "UNKN ";
        }
    }

    std::mutex m_mutex;
    LogLevel m_minLevel;
    bool m_consoleOutput;
    bool m_fileOutput;
    std::ofstream m_file;
};

// Convenience macros
#define LOG_TRACE(component, message)   ShaderGlass::Logger::Instance().Trace(component, message)
#define LOG_DEBUG(component, message)   ShaderGlass::Logger::Instance().Debug(component, message)
#define LOG_INFO(component, message)    ShaderGlass::Logger::Instance().Info(component, message)
#define LOG_WARNING(component, message) ShaderGlass::Logger::Instance().Warning(component, message)
#define LOG_ERROR(component, message)   ShaderGlass::Logger::Instance().Error(component, message)
#define LOG_FATAL(component, message)   ShaderGlass::Logger::Instance().Fatal(component, message)

// Performance timing macro
#define LOG_SCOPE_TIMER(component, operation) \
    ShaderGlass::Logger::ScopedTimer _scopedTimer##__LINE__(component, operation)

} // namespace ShaderGlass
