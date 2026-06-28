#pragma once

#include<iostream>
#include<string>

constexpr const char* COLOR_INFO    = "\033[37m";
constexpr const char* COLOR_WARN    = "\033[33m";
constexpr const char* COLOR_ERROR   = "\033[31m";
constexpr const char* COLOR_FATAL   = "\033[35m";
constexpr const char* COLOR_RESET   = "\033[0m";

enum class LogLevel {
	INFO,
	WARN,
	ERROR,
	FATAL
};


inline void Log(LogLevel level, const std::string& msg){
	const char* levelColor = COLOR_RESET;
	const char* levelStr = "UNKNOWN";
	switch(level)
	{
	case LogLevel::INFO:
		levelColor = COLOR_INFO;
		levelStr = "INFO";
		break;
	case LogLevel::WARN:
		levelColor = COLOR_WARN;
		levelStr = "WARN";
		break;
	case LogLevel::ERROR:
		levelColor = COLOR_ERROR;
		levelStr = "ERROR";
		break;
	case LogLevel::FATAL:
		levelColor = COLOR_FATAL;
		levelStr = "FATAL";
		break;
	default:
		break;
	}
	std::cout << levelColor << "[" << levelStr << "]" << msg << COLOR_RESET << std::endl;
} 


