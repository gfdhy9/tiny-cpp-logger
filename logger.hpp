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
constexpr size_t LOGGER_BUF_SIZE = 256;


inline void CloseFile(){
	std::lock_guard<std::mutex> lock(log_mtx);
	if(log_file.is_open())
		log_file.close();
}

struct LogFinalGuard
{
	~LogFinalGuard()
	{
		CloseFile();
	}
};
static LogFinalGuard log_guard;

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
	static bool log_inited = false;
	std::lock_guard<std::mutex> lock(log_mtx);
	if(log_inited){
		return;
	}
	std::string today = GetTodayDate();
	std::string filename = GetDailyLogName();
	current_log_date = today;
	log_file.open(filename, std::ios::out | std::ios::app);
	if(!log_file.is_open()){
		std::cout << "[FATAL] Log file open failed, no log will be saved!" << std::endl;
	}else{
		log_file << "==================== Program Start ====================" << std::endl;
	}
	log_inited = true; 
}

inline std::string GetTimeStamp(){
	std::time_t now = std::time(nullptr);
	std::tm local_tm = *std::localtime(&now);
	char buf[64] = {0};
	std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &local_tm);
	return std::string(buf);
}

inline void SetMinLogLevel(LogLevel level){
	std::lock_guard<std::mutex> lock(log_mtx);
	MIN_LOG_LEVEL = level;
}

inline LogLevel GetMinLogLevel(){
	std::lock_guard<std::mutex> lock(log_mtx);
	return MIN_LOG_LEVEL;
}

inline std::string FormatString(const char* fmt, ...){
	va_list args;
	va_start(args, fmt);
	char buf[LOGGER_BUF_SIZE] = {0};
	int ret = vsnprintf(buf, LOGGER_BUF_SIZE, fmt, args);
	va_end(args);
	if (ret < 0 || ret >= static_cast<int>(LOGGER_BUF_SIZE)){
		std::cout << "[WARN] Log message truncated, max length 256" << std::endl;
	}
	return std::string(buf);
}

inline std::string SanitizeMessage(const std::string& src){
	std::string res = src;
	for (char& c : res){
		if (c == '\n' || c == '\r' || c == '\t'){
			c = ' ';
		}
	}
	return res;
}

inline void Log (LogLevel level, int line, const std::string& msg){
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
		std::string newFile = GetDailyLogName();
		std::ofstream test(newFile, std::ios::out | std::ios::app);
		if (test.is_open()){
			test.close();
			log_file.close();
			log_file.open(newFile, std::ios::out | std::ios::app);
			current_log_date = today;
		}
		else{
			std::cout << "[FATAL] Failed to create new daily log file: " << newFile << std::endl;
		}
	}
	std::string time = GetTimeStamp();
	std::string safe_msg = SanitizeMessage(msg);
	std::string logContent = "[" + time + "] [" + levelStr + "] line " + std::to_string(line) + ": " + safe_msg; 
	if(log_file.is_open()){
		log_file << logContent << std::endl;
		log_file.flush();
	}
} 

inline bool ShouldPrintLog(LogLevel targetLevel){
	std::lock_guard<std::mutex> lock(log_mtx);
	return targetLevel >= MIN_LOG_LEVEL;
}

#define LOG_INFO(...)  do{ if(ShouldPrintLog(LogLevel::INFO)) Log(LogLevel::INFO, __LINE__, FormatString(__VA_ARGS__)); }while(0)
#define LOG_WARN(...)  do{ if(ShouldPrintLog(LogLevel::WARN)) Log(LogLevel::WARN, __LINE__, FormatString(__VA_ARGS__)); }while(0)
#define LOG_ERROR(...) do{ if(ShouldPrintLog(LogLevel::ERROR)) Log(LogLevel::ERROR, __LINE__, FormatString(__VA_ARGS__)); }while(0)
#define LOG_FATAL(...) do{ if(ShouldPrintLog(LogLevel::FATAL)) Log(LogLevel::FATAL, __LINE__, FormatString(__VA_ARGS__)); }while(0)
