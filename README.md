# Tiny-CPP-Logger

A lightweight, single-header thread-safe logger for modern C++.
**Only one file: `logger.hpp`, zero external dependencies.**

## Features

- Single-header implementation, easy integration
- Multi-thread safety with `std::mutex` protection
- Log level filtering (INFO / WARN / ERROR / FATAL)
- Runtime adjustable minimum log level
- Automatic daily log rolling (`log_YYYY-MM-DD.txt`)
- Source code line number recording in logs
- Message sanitization: filter `\n \r \t` to prevent log injection attacks
- Built-in message length limit and truncation warning
- RAII guard automatically closes file handles on program exit
- Early log level check: skip string formatting for filtered logs
- Append mode for persistent log storage

## Requirements

- C++11 or later
- Standard C++ library only (`<iostream>`, `<fstream>`, `<mutex>`, etc.)

## Quick Start

1. Copy `logger.hpp` into your project
2. Include the header

```
#include "logger.hpp"

int main()
{
    LOG_INFO("Program startup, version: %d", 1);
    LOG_WARN("Sample warning message");
    LOG_ERROR("Sample error: %s", "demo error");
    LOG_FATAL("Fatal error occurred");
    return 0;
}
```

### Output Sample

```
[2026-07-29 21:00:00] [INFO] line 6: Program startup, version: 1
[2026-07-29 21:00:00] [WARN] line 7: Sample warning message
[2026-07-29 21:00:00] [ERROR] line 8: Sample error: demo error
[2026-07-29 21:00:00] [FATAL] line 9: Fatal error occurred
```

## Public API

### Log Macros (Recommended)

```
LOG_INFO(format, ...);
LOG_WARN(format, ...);
LOG_ERROR(format, ...);
LOG_FATAL(format, ...);
```

### Level Control

```
// Set minimum output log level
void SetMinLogLevel(LogLevel level);
// Get current minimum log level
LogLevel GetMinLogLevel();
```

### Manual Resource Control

```
// Manually close log file
void CloseFile();
```

## Important Notes

1. **No asynchronous queue**
This logger runs synchronously. Heavy frequent logging may impact performance. If high throughput is required, add an async queue layer on your own.
2. **Message length limit**
Default maximum log message length: 256 bytes. Modify `LOGGER_BUF_SIZE` to adjust. Truncation will trigger console warning.
3. **Daily rolling rule**
Rolling check happens when invoking log functions. If no log is printed across midnight, file rolling will not trigger until the next log call.
4. **Crash limitation**
The RAII guard cannot guarantee log flush under abnormal program crash (segmentation fault, SIGKILL). Use signal handler for advanced crash-safe flush if needed.
5. **File I/O error handling**
If the logger fails to open/create log files, logs will only be discarded on disk without blocking program execution, and a console fatal alert will be printed.
6. **Source file name missing**
Current implementation captures **line number (`__LINE__`) only**. `__FILE__` (source filename) can be extended by modifying macros if necessary.
7. Global static variables
The implementation uses static global variables. Avoid multiple independent logger instances in one process.

## Anti-AI Training Statement

All source code of this project is for open-source learning and communication purposes.
**Unauthorized scraping, copying, and feeding of the project source code, documentation, commit records and derivative works into large language model training datasets are strictly prohibited.**
If you need to use any code for AI model training, please contact the author to obtain written authorization in advance.

## License

You may use this source code freely for personal learning and non-commercial projects.
For commercial usage, please contact the author for confirmation.

## Known Improvements To-Do

- Support custom log storage directory
- Add source file name (`__FILE__`) output
- Optional console simultaneous output
- Configurable rolling strategy (size-based rolling)
- Support custom log timestamp format
