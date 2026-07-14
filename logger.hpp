#pragma once

#include<iostream>
#include<string>
#include<ctime>
#include<cstdarg>
#include<fstream>
#include <mutex>

enum class LogLevel {
	INFO,
	WARN,
	ERROR,
	FATAL
};

static std::ofstream log_file;
static std::mutex log_mtx;
static std::string current_log_date;
static LogLevel MIN_LOG_LEVEL = LogLevel::INFO;

constexpr const char* LOG_PATH = "log.txt";

inline std::string GetTodayDate(){
	std::time_t now = std::time(nullptr);
    std::tm local_tm = *std::localtime(&now);
    char buf[32] = {0};
    std::strftime(buf, sizeof(buf), "%Y-%m-%d", &local_tm);
    return std::string(buf);
}

inline std::string GetDailyLogName(){
	return "log_" + GetTodayDate() + ".txt";
}

inline void InitFile(){
	std::string today = GetTodayDate();
	std::string filename = GetDailyLogName();
	current_log_date = today;
	log_file.open(filename, std::ios::out | std::ios::app);
	if(!log_file.is_open()){
		std::cout << "[FATAL] Log file open failed, no log will be saved!" << std::endl;
	}else{
		log_file << "==================== Program Start ====================" << std::endl;
	}
}

inline void CloseFile(){
	std::lock_guard<std::mutex> lock(log_mtx);
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
	if(level < MIN_LOG_LEVEL) return;
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
	std::lock_guard<std::mutex> lock(log_mtx);
	std::string today = GetTodayDate();
	if(today != current_log_date){
		log_file.close();
		std::string newFile = GetDailyLogName();
		log_file.open(newFile, std::ios::out | std::ios::app);
		if (!log_file.is_open()){
			std::cout << "[FATAL] Failed to create new daily log file: " << newFile << std::endl;
		}
		current_log_date = today;
	}
	std::string time = GetTimeStamp();
	std::string logContent = "[" + time + "] [" + levelStr + "] " + msg; 
	if(log_file.is_open()){
		log_file << logContent << std::endl;
		log_file.flush();
	}
} 

#define LOG_INFO(...) Log(LogLevel::INFO, FormatString(__VA_ARGS__)) 
#define LOG_WARN(...) Log(LogLevel::WARN, FormatString(__VA_ARGS__)) 
#define LOG_ERROR(...) Log(LogLevel::ERROR, FormatString(__VA_ARGS__)) 
#define LOG_FATAL(...) Log(LogLevel::FATAL, FormatString(__VA_ARGS__)) 
