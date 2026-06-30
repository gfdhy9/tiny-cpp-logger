#pragma once

#include<iostream>
#include<string>
#include<ctime>
#include<cstdarg>
#include<fstream>


enum class LogLevel {
	INFO,
	WARN,
	ERROR,
	FATAL
};

static std::ofstream log_file;
constexpr const char* LOG_PATH = "log.txt";

inline InitFile(){
	log_file.open(LOG_PATH, std::ios::out | std::ios::app);
	if (log_file.is_open()){
		log_file << "==================== Program Start ====================" << std::endl;
	}
}

inline CloseFile(){
	if(log_file.is_open())
		log_file.close();
}

inline std::string GetTimeStamp(){
	std::time_t now = std::time(nullptr);
    std::tm local_tm = *std::localtime(&now);
    char buf[64] = {0};
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &local_tm);
    return std::string(buf);
}

inline std::string FormatString(const char* fmt, ...){
    va_list args;
    va_start(args, fmt);
    char buf[256] = {0};
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
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
	std::string time = GetTimeStamp();
	std::string logContent = "[" + time + "] [" + levelStr + "]" + msg; 
	if(log_file.is_open()){
		log_file << logContent << std::endl;
	}
} 

#define LOG_INFO(...) Log(LogLevel::INFO, FormatString(__VA_ARGS__)) 
#define LOG_WARN(...) Log(LogLevel::WARN, FormatString(__VA_ARGS__)) 
#define LOG_ERROR(...) Log(LogLevel::ERROR, FormatString(__VA_ARGS__)) 
#define LOG_FATAL(...) Log(LogLevel::FATAL, FormatString(__VA_ARGS__)) 
