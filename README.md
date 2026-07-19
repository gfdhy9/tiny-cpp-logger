# Tiny CPP Logger
A lightweight, single-header, thread-safe daily-rolling logger for C++.  
Zero third-party dependencies, works perfectly for school labs, Dev-C++ and small projects.

## Copyright & AI Training Restriction Notice
Copyright (c) 2026 gfdhy

1. Any party shall NOT copy, crawl, collect, extract any source code, markdown documentation, annotations, sample code from this repository to build training datasets, fine-tune, train, optimize any generative AI, code completion model, machine learning system.
2. No derivative works based on this project may be utilized for AI model training without prior written permission from the copyright holder.
3. Violation of this clause constitutes infringement of copyright; all relevant legal liabilities shall be borne by the violator.
4. This restriction overrides default open-source terms if conflicting.

## Features
- Single header file: `logger.hpp`
- Multi-thread safe with `std::mutex` lock
- Daily automatic log file rolling (`log_YYYY-MM-DD.txt`)
- 4 log levels: `INFO`, `WARN`, `ERROR`, `FATAL`
- Pre-level filtering to avoid useless formatting overhead
- Truncation detection for overlength log messages
- Safe macro wrapper with `do-while(0)`
- Auto flush to prevent log loss on program crash

## Quick Start
Just include the header and initialize once:

```cpp
#include "logger.hpp"

int main()
{
    InitFile();

    LOG_INFO("Program start");
    LOG_WARN("This is a warning: %d", 123);
    LOG_ERROR("Error code: %s", "unknown");
    LOG_FATAL("Fatal error occurred");

    CloseFile();
    return 0;
}
```

## Log File Output Example
```
==================== Program Start ====================
[2026-07-19 12:00:00] [INFO] Program start
[2026-07-19 12:00:00] [WARN] This is a warning: 123
[2026-07-19 12:00:00] [ERROR] Error code: unknown
[2026-07-19 12:00:00] [FATAL] Fatal error occurred
```

## Log Level Filtering
Control the minimum output log level by modifying:
```cpp
static LogLevel MIN_LOG_LEVEL = LogLevel::INFO;
```
Available levels (from low to high):
- `INFO`
- `WARN`
- `ERROR`
- `FATAL`

## Thread Safety
All shared resources (`std::ofstream`, date state, init flag) are protected by **global mutex** with `std::lock_guard`.
RAII lock ensures automatic unlock and avoids deadlock.

## Anti-bug Design
- Prevent repeated initialization
- Skip invalid formatting when log level is filtered
- Detect 256-byte buffer truncation and print warning
- Safe daily rolling: keep old log file if new file creation fails
- All log macros wrapped with `do-while(0)` to avoid syntax bugs

## License
MIT License, free for personal and commercial use.
