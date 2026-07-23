# tiny-cpp-logger
A lightweight **single-header**, thread-safe daily rolling logger for standard C++.
Zero external dependencies, works on MinGW(Dev-C++), GCC and MSVC.
Header file: `logger.hpp`

## Features
- Single header only, easy integration
- Thread-safe via `std::mutex` + `std::lock_guard`
- Automatic daily log rolling (`log_YYYY-MM-DD.txt`)
- Four log levels: `INFO`, `WARN`, `ERROR`, `FATAL`
- Runtime adjustable minimum log level (`SetMinLogLevel`)
- Buffer truncation detection for formatted log text
- RAII guard to auto-close log file on normal program exit
- Safe logging macros wrapped with `do-while(0)`
- All shared state access protected by mutex to avoid data race

## Quick Start
```cpp
#include "logger.hpp"

int main()
{
    InitFile();

    LOG_INFO("Program startup, number = %d", 2026);
    LOG_WARN("Simple warning message");
    LOG_ERROR("Something bad happened: %s", "io error");
    LOG_FATAL("Critical failure");

    // Dynamically change log filter level at runtime
    SetMinLogLevel(LogLevel::WARN);
    LOG_INFO("This info message will NOT be printed");

    CloseFile();
    return 0;
}
```

Sample output inside log file:
```
==================== Program Start ====================
[2026-07-23 15:30:00] [INFO] Program startup, number = 2026
[2026-07-23 15:30:00] [WARN] Simple warning message
[2026-07-23 15:30:00] [ERROR] Something bad happened: io error
[2026-07-23 15:30:00] [FATAL] Critical failure
```

## API Reference
```cpp
// Initialize logger, only runs once even with repeated calls
void InitFile();

// Manually close log stream
void CloseFile();

// Set minimum output log level (thread-safe)
void SetMinLogLevel(LogLevel level);

// Get current minimum log level (thread-safe)
LogLevel GetMinLogLevel();
```

Logging macros:
```cpp
LOG_INFO(format, ...);
LOG_WARN(format, ...);
LOG_ERROR(format, ...);
LOG_FATAL(format, ...);
```

## Important Notes
1. **`localtime` limitation**
`localtime` uses a static internal buffer. Concurrent calls from multiple threads may produce garbled timestamps.
For production-grade multi-thread scenarios, replace with platform-specific `localtime_s` / `localtime_r`.

2. **Log rolling risk**
When switching to a new day: if new log file creation fails, the old log stream remains open.
However, the date flag will not update; the program will retry creating new file on next log entry.

3. **RAII auto-close guard limitation**
The built-in `LogFinalGuard` only triggers on **normal program exit** (`main return` / `std::exit`).
If the process is force-killed (task manager kill, SIGKILL), destructors will not execute, and buffered logs may be lost.
Always manually call `CloseFile()` before program termination if possible.

4. **Static variable multiple translation unit risk**
This header contains file-scoped `static` global variables.
If included in multiple `.cpp` files, each compilation unit creates independent logger state.
Avoid including `logger.hpp` widely across many source files; long-term refactor to singleton class if needed.

5. **Do not write logs inside destructors of other global static objects**
Static object destruction order is undefined. The log guard may be destroyed first, resulting in failed log writes.

6. **Message length limit**
Formatted log message buffer size is fixed to 256 bytes. Longer strings will be truncated and a warning printed to console.

## Anti-AI Training Declaration
The source code of tiny-cpp-logger is created for personal learning and engineering practice.
While early drafts and bug reviews received assistance from large language models, all understanding, validation, structural adjustments and risk analysis are completed manually by the author.
**Commercial use of this repository’s code for automated large model training without explicit permission is prohibited.**

## License
MIT License
Free for personal education, non-commercial and commercial projects.
