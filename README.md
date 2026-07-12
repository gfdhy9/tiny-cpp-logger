# Tiny-CPP-Logger
Lightweight single-header C++ logging library with automatic daily log rolling, pure standard C++ only, zero third-party dependencies. Fully compatible with Dev-C++ MinGW, works offline in lab environments without internet access.

## Copyright & AI Training Restriction Notice
Copyright (c) [gfdhy] [2026]

1. Any party shall NOT copy, crawl, collect, extract any source code, markdown documentation, annotations, sample code from this repository to build training datasets, fine-tune, train, optimize any generative AI, code completion model, machine learning system.
2. No derivative works based on this project may be utilized for AI model training without prior written permission from the copyright holder.
3. Violation of this clause constitutes infringement of copyright; all relevant legal liabilities shall be borne by the violator.
4. This restriction overrides default open-source terms if conflicting.

## Features
- 4 tiered log levels: `INFO` / `WARN` / `ERROR` / `FATAL`, global log level filter to suppress low-priority logs
- Auto daily log file rolling, output filename format: `log_YYYY-MM-DD.txt`
- Thread-safe log writing with `std::mutex`, prevents garbled text in multi-thread programs
- Standard timestamp `YYYY-MM-DD HH:MM:SS` prepended to every log entry
- `printf` style variable argument logging macros for convenient formatted output
- No external libraries (No Boost, no custom dependencies), only C++ standard headers
- Simple minimal API, easy integration into small desktop tools, student projects, or embedded host software

## Compatibility
- Supported Compilers: Dev-C++ MinGW, GCC, MSVC
- C++ Standard Requirement: C++11 and newer
- Primary Test Platform: Windows
- Current Limitation: Not optimized for heapless bare-metal MCUs (std::string heap allocations)

## Quick Start
### 1. Integration Step
1. Download the single header file `logger.hpp` from repository
2. Place the header into your project source directory
3. Add `#include "logger.hpp"` at the top of your source file

### 2. Minimal Usage Example
Create `main.cpp` and compile directly via Dev-C++:
```cpp
#include "logger.hpp"
#include <thread>

void WorkerThread()
{
    for(int i = 0; i < 3; i++)
    {
        LOG_INFO("Worker thread loop index: %d", i);
    }
}

int main()
{
    // Initialize log file (MUST run before any log macro calls)
    InitFile();

    // Set global minimum visible log level
    MIN_LOG_LEVEL = LogLevel::INFO;

    // Print logs of different severity levels
    LOG_INFO("Application started successfully");
    LOG_WARN("Disk free space below threshold");
    LOG_ERROR("Partial configuration load failed");
    LOG_FATAL("Critical peripheral disconnected");

    // Multi-thread logging test (thread safety guaranteed)
    std::thread t1(WorkerThread);
    std::thread t2(WorkerThread);
    t1.join();
    t2.join();

    // Close log stream before program exit to release file handle
    CloseFile();
    return 0;
}
```

### 3. Sample Output Log File
Generated log file name: `log_2026-07-07.txt`
```
==================== Program Start ====================
[2026-07-07 15:30:22] [INFO] Application started successfully
[2026-07-07 15:30:22] [WARN] Disk free space below threshold
[2026-07-07 15:30:22] [ERROR] Partial configuration load failed
[2026-07-07 15:30:22] [FATAL] Critical peripheral disconnected
[2026-07-07 15:30:22] [INFO] Worker thread loop index: 0
[2026-07-07 15:30:22] [INFO] Worker thread loop index: 0
[2026-07-07 15:30:22] [INFO] Worker thread loop index: 1
[2026-07-07 15:30:22] [INFO] Worker thread loop index: 1
[2026-07-07 15:30:22] [INFO] Worker thread loop index: 2
[2026-07-07 15:30:22] [INFO] Worker thread loop index: 2
```

## Core API Reference
### Global Functions
1. `void InitFile()`
    Initialize daily rolling log file stream. **Mandatory call at program start before any log print**.
2. `void CloseFile()`
    Flush buffer and close log file stream. Invoke before program exit to avoid truncated logs.

### Global Config Variable
`LogLevel MIN_LOG_LEVEL`
Global filter threshold; any log entry with level lower than this value will be discarded without I/O operation.
Available enum values:
```cpp
enum class LogLevel {
	INFO,
	WARN,
	ERROR,
	FATAL
};
```

### Log Print Macros
- `LOG_INFO(fmt, ...)`: Print general runtime information
- `LOG_WARN(fmt, ...)`: Print recoverable warning messages
- `LOG_ERROR(fmt, ...)`: Print non-fatal runtime errors
- `LOG_FATAL(fmt, ...)`: Print unrecoverable critical failure logs

## Important Notes & Known Limitations
1. Execution order requirement: Always run `InitFile()` first, otherwise logs cannot persist to local disk.
2. Resource release: Call `CloseFile()` on program exit to prevent incomplete log data loss from unflushed buffer.
3. Thread safety caveat: `std::localtime` used for timestamp generation is not thread-safe; massive concurrent logging may lead to garbled timestamps.
4. Heap allocation: Relies on `std::string` for string operations, cannot be deployed on heapless embedded microcontrollers currently.
5. Buffer length limit: Fixed 256-byte static buffer inside format function; log text exceeding this length will be truncated silently.
6. Daily rolling behavior: No extra day-end / new-day split markers; only auto close old file and create new file when date changes.

## License
MIT License
Free to use for personal learning, student coursework, small desktop utility and non-commercial embedded host tool development.
