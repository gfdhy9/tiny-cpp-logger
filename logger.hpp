#pragma once

#include<iostream>
#include<string>
#include<ctime>
#include<cstdarg>

enum class LogLevel {
	INFO,
	WARN,
	ERROR,
	FATAL
};

inline std::string GetTimeStamp(){
	std::time_t now = std::time(nullptr);
    std::tm local_tm = *std::localtime(&now);
    char buf[64] = {0};
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &local_tm);
    return std::string(buf);
}
inline void Log (LogLevel level, const std::string& msg){
	const char* levelStr = "UNKNOWN";
	switch (level)
	{
	case LogLevel::INFO:
		levelStr = "INFO";
		break;
	case LogLevel::WARN:
		levelStr = "WARN";
		break;
	case LogLevel::ERROR:
		levelStr = "ERROR";
		break;
	case LogLevel::FATAL:
		levelStr = "FATAL";
		break;
	default:
		break;
	}
	std::cout << "[" << GetTimeStamp() << "] [" << levelStr << "]" << msg << std::endl;
} 


