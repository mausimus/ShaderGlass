# Logging Infrastructure Guide

ShaderGlass includes a lightweight, thread-safe logging system for debugging and diagnostics.

## Quick Start

```cpp
#include "Util/Logger.h"
using namespace ShaderGlass;

// Initialize logger (in WinMain or startup code)
Logger::Instance().SetLevel(LogLevel::DEBUG);
Logger::Instance().SetFile("Logs/shaderglass.log");
Logger::Instance().SetFileOutput(true);

// Use convenience macros
LOG_INFO("CaptureManager", "Starting capture session");
LOG_WARNING("ShaderGlass", "Shader compilation took longer than expected");
LOG_ERROR("DeviceCapture", "Failed to initialize webcam");

// Performance timing
{
    LOG_SCOPE_TIMER("ShaderPass", "Render");
    // Code to measure
    RenderFrame();
} // Automatically logs execution time
```

## Log Levels

| Level | Purpose | Example Use Case |
|-------|---------|------------------|
| **TRACE** | Very detailed debug info | Frame-by-frame state dumps |
| **DEBUG** | Debug information | Performance metrics, state changes |
| **INFO** | General information | Startup messages, configuration |
| **WARNING** | Warning messages | Recoverable errors, deprecated usage |
| **ERROR** | Error messages | Failed operations, exceptions |
| **FATAL** | Fatal errors | Unrecoverable crashes |

## Configuration

### Setting Log Level
```cpp
// Only show INFO and above (filters out DEBUG and TRACE)
Logger::Instance().SetLevel(LogLevel::INFO);

// Show everything (for debugging)
Logger::Instance().SetLevel(LogLevel::TRACE);

// Only critical messages
Logger::Instance().SetLevel(LogLevel::ERROR);
```

### Output Destinations
```cpp
// Console output (OutputDebugString - visible in VS debugger)
Logger::Instance().SetConsoleOutput(true);

// File output
Logger::Instance().SetFile("Logs/shaderglass.log");
Logger::Instance().SetFileOutput(true);

// Both
Logger::Instance().SetConsoleOutput(true);
Logger::Instance().SetFileOutput(true);
```

## Usage Examples

### Basic Logging
```cpp
// Component name helps identify log source
LOG_INFO("ShaderWindow", "Window created successfully");
LOG_DEBUG("CaptureManager", "Frame rate: 60 FPS");
LOG_ERROR("ShaderCompiler", "Failed to compile shader: " + errorMsg);
```

### Conditional Logging
```cpp
if (FAILED(hr))
{
    std::stringstream ss;
    ss << "CreateTexture2D failed with HRESULT: 0x"
       << std::hex << hr;
    LOG_ERROR("ShaderGlass", ss.str());
}
```

### Performance Measurement
```cpp
void ExpensiveOperation()
{
    LOG_SCOPE_TIMER("Performance", "ExpensiveOperation");

    // Your code here
    ProcessComplexData();

} // Logs: "[DEBUG] [Performance] ExpensiveOperation took 1234 μs"
```

### Structured Logging
```cpp
std::stringstream ss;
ss << "Shader preset loaded: " << presetName
   << ", passes: " << numPasses
   << ", feedback: " << (requiresFeedback ? "yes" : "no");
LOG_INFO("PresetManager", ss.str());
```

## Log File Format

```
2025-02-02 15:30:45.123 [INFO ] [CaptureManager] Starting capture session
2025-02-02 15:30:45.125 [DEBUG] [ShaderGlass] Creating swap chain: 1920x1080
2025-02-02 15:30:45.156 [WARN ] [ShaderPass] Constant buffer update skipped
2025-02-02 15:30:45.234 [ERROR] [DeviceCapture] Device not found: webcam0
```

Format: `YYYY-MM-DD HH:MM:SS.mmm [LEVEL] [Component] Message`

## Best Practices

### DO ✅
- **Use descriptive component names**: "CaptureManager", "ShaderCompiler", "DirectX"
- **Include context**: Log values, states, error codes
- **Use appropriate levels**: Don't log INFO as ERROR
- **Log critical paths**: Initialization, errors, state changes
- **Use SCOPE_TIMER**: For performance-critical operations

### DON'T ❌
- **Avoid hot path logging**: Don't log every frame in render loop
- **Don't log sensitive data**: Passwords, API keys, personal info
- **Avoid excessive TRACE**: Can impact performance
- **Don't log in tight loops**: Use counters instead
- **Avoid blocking operations**: Logger is thread-safe but still locks

## Performance Considerations

### Low-Overhead Logging
```cpp
// Good: Only log state changes
if (newState != oldState)
{
    LOG_DEBUG("StateMachine", "State changed: " + stateToString(newState));
}

// Bad: Logging every frame
for (auto& frame : frames)
{
    LOG_DEBUG("Render", "Processing frame"); // DON'T DO THIS
}
```

### Conditional Compilation
```cpp
#ifdef _DEBUG
    LOG_DEBUG("ShaderGlass", "Debug info: " + expensiveStringConstruction());
#endif
```

### Sampling
```cpp
static int frameCounter = 0;
if (++frameCounter % 60 == 0) // Log once per second at 60 FPS
{
    LOG_INFO("Performance", "FPS: " + std::to_string(currentFPS));
}
```

## Integration with Error Handling

```cpp
try
{
    RiskyOperation();
}
catch (const std::exception& e)
{
    LOG_ERROR("ErrorHandler", std::string("Exception caught: ") + e.what());
    // Handle error
}
catch (...)
{
    LOG_FATAL("ErrorHandler", "Unknown exception caught");
    throw;
}
```

## Viewing Logs

### Visual Studio Debugger
- Logs appear in **Output** window (Debug → Windows → Output)
- Filter by searching for component names

### Log Files
- Located in `Logs/` directory (created automatically)
- Open with any text editor
- Use tail/grep for real-time monitoring:
  ```bash
  # PowerShell
  Get-Content Logs/shaderglass.log -Wait -Tail 50

  # Search for errors
  Select-String -Path Logs/shaderglass.log -Pattern "ERROR"
  ```

## Thread Safety

The logger is **fully thread-safe**:
- Uses `std::mutex` for synchronization
- Safe to call from multiple threads simultaneously
- File writes are serialized

## Future Enhancements

Potential improvements for production use:
- Async logging (lock-free queue)
- Log rotation (size/time-based)
- Remote logging (network sink)
- Structured logging (JSON format)
- Log filtering by component
- Performance counters aggregation

## Example: Production Configuration

```cpp
void InitializeLogging()
{
    auto& logger = Logger::Instance();

#ifdef _DEBUG
    logger.SetLevel(LogLevel::DEBUG);
    logger.SetConsoleOutput(true);
#else
    logger.SetLevel(LogLevel::INFO);
    logger.SetConsoleOutput(false);
#endif

    // Generate timestamped log file
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm tm;
    localtime_s(&tm, &time);

    std::stringstream filename;
    filename << "Logs/shaderglass_"
             << std::put_time(&tm, "%Y%m%d_%H%M%S")
             << ".log";

    logger.SetFile(filename.str());
    logger.SetFileOutput(true);

    LOG_INFO("Application", "ShaderGlass v1.2.3 starting");
    LOG_INFO("Application", "Log file: " + filename.str());
}
```
