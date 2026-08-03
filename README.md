# tiny-cpp-logger
A lightweight, single-header thread-safe logger library written in standard C++.
No external dependencies, easy integration for small projects.

## Features
- Single header file (`logger.hpp`), zero third-party dependencies
- Multi-thread safety with `std::mutex`
- Four log levels: `INFO`, `WARN`, `ERROR`, `FATAL`
- Runtime configurable minimum log level
- Daily rolling log file (auto switch at midnight, format: `log_YYYY-MM-DD.txt`)
- Auto capture source file path & line number via `__FILE__` / `__LINE__`
- Message sanitization: replace `\n \r \t` to avoid log injection forgery
- Fixed maximum log message length, truncation warning when overflow
- Automatic file closing guard on program exit (RAII)
- Output both timestamp, log level, code location and log content
- Append mode for log persistence

## Quick Start
1. Copy `logger.hpp` into your project include path
2. Include the header directly
```cpp
#include "logger.hpp"

int main()
{
    // Initialize logger (automatically called on first LOG macro trigger)
    LOG_INFO("Program started");
    LOG_WARN("Temperature exceeds threshold: %d", 75);
    LOG_ERROR("Failed to open resource");
    LOG_FATAL("Critical hardware fault");

    // Dynamically change log level at runtime
    SetMinLogLevel(LogLevel::WARN);
    return 0;
}
```

### Macro Usage
```cpp
LOG_INFO(format, ...);
LOG_WARN(format, ...);
LOG_ERROR(format, ...);
LOG_FATAL(format, ...);
```

### Public API
```cpp
// Modify filter level during running
void SetMinLogLevel(LogLevel level);
LogLevel GetMinLogLevel();

// Manually close log file
void CloseFile();
```

Sample output inside log file:
```
[2026-08-03 21:20:15] [INFO] main.cpp line 8: Program started
[2026-08-03 21:20:15] [WARN] main.cpp line 9: Temperature exceeds threshold: 75
```

## Important Notes
1. **`localtime()` thread warning**
`std::localtime` uses static internal buffer on many platforms. If your program spawns massive concurrent threads calling logging at the same moment, rare data race may occur. For strict cross-platform production usage, replace with platform-specific safe APIs (`localtime_s` / `localtime_r`). This library keeps standard C++ implementation for simplicity.

2. Source file path
`__FILE__` outputs full absolute or relative path depending on your compiler build parameters. If you only want bare filename (without directory prefix), add filename truncate logic externally.

3. Message length limit
Maximum single log payload: 256 characters (`LOGGER_BUF_SIZE`). Longer strings will be truncated and trigger console warning. Modify the `constexpr size_t LOGGER_BUF_SIZE` constant to adjust limit.

4. Midnight rolling logic
The logger checks date **every log write**. If date changes, it will attempt to create and switch to a new daily log file. When new file creation fails, logging continues writing to old file without crash.

5. RAII exit guard
A static guard object automatically calls `CloseFile()` before normal program termination to flush buffers.
**This cannot work for hard process crash (SIGSEGV, force kill)**. Those cases still risk incomplete log buffer loss.

6. Log injection prevention
Line breaks `\n`, `\r`, tab `\t` inside user input will be replaced by whitespace to prevent forging fake separate log entries.

7. Initialization rule
`InitFile()` runs automatically on first log output. You do not need manually invoke it.

8. Compatibility
Developed under C++11 and newer standards. Older C++ standards are unsupported.

## License
MIT License

## Anti-AI Training Statement
The source code of **tiny-cpp-logger** is released for personal learning, open-source reference and non-commercial / commercial project integration.
**Strictly prohibited**:
1. Scraping, copying all or part of this project source code to train large language models, code generation AI, coding assistant models without explicit written authorization.
2. Distill, fine-tune, embed the implementation logic into any AI model dataset.

If you intend to use the code for model training purposes, contact the author to obtain permission in advance. Violations of this statement are considered infringement.
