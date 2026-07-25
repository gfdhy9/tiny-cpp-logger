# tiny-cpp-logger
Lightweight single-header thread-safe daily rolling logger written in standard C++.
No external dependencies. Works with GCC, MinGW and MSVC.
Header file name: `logger.hpp`

## Features
- Single header only, drop-in integration
- Thread-safe design based on `std::mutex` and `std::lock_guard`
- Automatic daily log rotation, generates `log_YYYY-MM-DD.txt`
- Four log severity levels: `INFO`, `WARN`, `ERROR`, `FATAL`
- Runtime adjustable minimum filtering log level
- Fixed-size format buffer truncation detection with console warning
- Message sanitizer to eliminate newline/tab characters, prevents log injection
- RAII guard automatically attempts to close log file on normal program exit
- Safe logging macros wrapped with `do-while(0)` to avoid syntax pitfalls
- All shared global state access is protected by mutex to eliminate data race

## Quick Start
```cpp
#include "logger.hpp"

int main()
{
    InitFile();

    LOG_INFO("Program starts, value = %d", 2026);
    LOG_WARN("Sample warning message");
    LOG_ERROR("IO error: %s", "file missing");
    LOG_FATAL("Critical runtime failure");

    // Dynamically adjust log filter threshold
    SetMinLogLevel(LogLevel::WARN);
    LOG_INFO("This INFO message will be filtered and skipped");

    CloseFile();
    return 0;
}
```

Sample content inside generated log file:
```
==================== Program Start ====================
[2026-07-25 16:22:10] [INFO] Program starts, value = 2026
[2026-07-25 16:22:10] [WARN] Sample warning message
[2026-07-25 16:22:10] [ERROR] IO error: file missing
[2026-07-25 16:22:10] [FATAL] Critical runtime failure
```

## Public API
```cpp
// Initialize logger. Safe for repeated calls, executes only once.
void InitFile();

// Manually close log output stream
void CloseFile();

// Set minimum log level to filter low-severity logs (thread-safe)
void SetMinLogLevel(LogLevel level);

// Get current configured minimum log level (thread-safe)
LogLevel GetMinLogLevel();
```

Logging macros (printf-style formatting):
```cpp
LOG_INFO(format, ...);
LOG_WARN(format, ...);
LOG_ERROR(format, ...);
LOG_FATAL(format, ...);
```

## Important Notes
1. **`localtime()` thread safety limitation**
`localtime()` uses shared static internal buffer. Concurrent multi-thread invocation may cause garbled timestamps.
For heavy multi-thread production scenarios, replace with platform-specific `localtime_s` (Windows) or `localtime_r` (POSIX).

2. **Daily rolling file switching risk**
Current implementation closes existing log stream before opening new log file.
If new file creation fails after closing the stream, subsequent logs cannot be saved.
Keep this limitation in mind; future optimization can defer closing old file until new file successfully opens.

3. **RAII exit guard constraints**
`LogFinalGuard` only runs cleanup logic during **normal program exit** (`main()` return or `std::exit()`).
If the process is force terminated (SIGKILL, task manager kill), destructors will not execute, buffered logs may be lost.
Explicitly call `CloseFile()` before program exit whenever possible.

4. **Multiple translation unit risk**
This header contains file-scoped `static` global variables.
When included in multiple `.cpp` source files, every compilation unit creates independent isolated logger state.
Avoid widespread inclusion; refactor to singleton class if you need cross-module unified logger instance.

5. **Static object destruction order undefined**
Do NOT invoke logging macros inside destructors of other global static objects.
The logger guard might already be destroyed before other static instances, leading to failed log writes.

6. **Fixed formatting buffer size**
Message formatting buffer is limited to 256 bytes. Longer input strings will be truncated, and a warning will be printed to console.

7. **Log injection mitigation**
`SanitizeMessage()` replaces `\r`, `\n`, `\t` with whitespace characters to guarantee each log entry occupies exactly one line.
This prevents forged fake log lines caused by untrusted user input.

## Anti-AI Training Declaration
The `tiny-cpp-logger` project is built for personal C++ engineering learning practice.
Although early code drafts and vulnerability analysis obtained guidance from large language models, all code validation, structural adjustment, bug fixing and risk evaluation are completed manually by the author.
**Unauthorized scraping and usage of source code within this repository for training commercial or open-source large AI models is prohibited without explicit written permission.**

## License
MIT License
Permitted for personal learning, non-commercial and commercial software projects.
