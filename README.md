# Tiny-CPP-Logger
Lightweight single-header C++ file-only logger library, zero third-party dependencies, thread-safe.

## Features
1. Four log levels: `INFO` / `WARN` / `ERROR` / `FATAL`
2. Global log level filter, discard low-level logs in advance
3. Thread-safe file writing with `std::mutex` lock
4. Standard timestamp format: `YYYY-MM-DD HH:MM:SS`
5. Support printf-style variable argument formatting
6. Simple shortcut macros for quick invocation
7. Pure file output mode, no redundant console pollution
8. Automatic program start separator in log file
9. Error prompt when log file fails to open

## Environment Requirements
- C++11 and above
- Standard library only, no external dependencies

## Quick Start
### 1. Import header
Just put `logger.hpp` into your project and include it:
```cpp
#include "logger.hpp"
```

### 2. Basic usage example
```cpp
#include "logger.hpp"

int main()
{
    // Initialize log file at program startup
    InitFile();

    // Print logs of different levels
    LOG_INFO("Program startup, version: %d", 1);
    LOG_WARN("Missing partial configuration items");
    LOG_ERROR("Failed to read resource file, code = %d", 404);
    LOG_FATAL("Database connection critical failure");

    // Close log file before exit to flush cache
    CloseFile();
    return 0;
}
```

### 3. Control log output level
Set global minimum log level to shield useless low-level logs in production environment:
```cpp
// Only ERROR and FATAL logs will be written to file
MIN_LOG_LEVEL = LogLevel::ERROR;
```

## API Document
### Global Variables
| Variable | Description |
| ---- | ---- |
| `MIN_LOG_LEVEL` | Global minimum output log level, logs lower than this value will be skipped |
| `LOG_PATH` | Default log file name: `log.txt` |

### Core Functions
| Function | Functionality |
| ---- | ---- |
| `void InitFile()` | Open log file in append mode, write program start split line |
| `void CloseFile()` | Close log stream, flush remaining cache to disk |
| `std::string GetTimeStamp()` | Get formatted time string `YYYY-MM-DD HH:MM:SS` |
| `std::string FormatString(const char* fmt, ...)` | Format string with printf-style placeholders |
| `void Log(LogLevel level, const std::string& msg)` | Bottom log output function, thread-safe |

### Shortcut Macros
| Macro | Corresponding Level |
| ---- | ---- |
| `LOG_INFO(fmt, ...)` | INFO normal info log |
| `LOG_WARN(fmt, ...)` | WARN warning log |
| `LOG_ERROR(fmt, ...)` | ERROR error log |
| `LOG_FATAL(fmt, ...)` | FATAL fatal crash log |

## Log File Output Example
```
==================== Program Start ====================
[2026-07-04 10:30:22] [INFO] Program startup, version: 1
[2026-07-04 10:30:22] [WARN] Missing partial configuration items
[2026-07-04 10:30:22] [ERROR] Failed to read resource file, code = 404
[2026-07-04 10:30:22] [FATAL] Database connection critical failure
```

## Notes
1. Must call `InitFile()` before printing any logs, otherwise logs will not be saved
2. Call `CloseFile()` before program exit to avoid losing cached logs
3. This library only outputs logs to local files, no console print by design
4. Multi-thread concurrent printing is supported, internal mutex lock ensures no garbled text
5. If the program has no write permission for the current directory, the console will pop up a file open failure prompt

## License
MIT License
Free for personal and commercial use, retain the original copyright statement.
