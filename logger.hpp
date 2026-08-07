#pragma once

#include<iostream>
#include<string>
#include<ctime>
#include<cstdarg>
#include<fstream>
#include<mutex>
#include<cstring>

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
static bool full_path_on = false; 
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

inline std::string GetTodayDate(const std::tm& local_tm){
	char buf[32] = {0};
	std::strftime(buf, sizeof(buf), "%Y-%m-%d", &local_tm);
	return std::string(buf);
}

inline std::string GetTimeStamp(const std::tm& local_tm){
	char buf[64] = {0};
	std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &local_tm);
	return std::string(buf);
}

inline std::string GetDailyLogName(const std::tm& local_tm){
	return "log_" + GetTodayDate(local_tm) + ".txt";
}

inline void InitFile(){
	static bool log_inited = false;
	std::lock_guard<std::mutex> lock(log_mtx);
	if(log_inited){
		return;
	}
	std::time_t now_ts = std::time(nullptr);
	std::tm local_tm = *std::localtime(&now_ts);
	std::string today = GetTodayDate(local_tm);
	std::string filename = GetDailyLogName(local_tm);
	current_log_date = today;
	log_file.open(filename, std::ios::out | std::ios::app);
	if(!log_file.is_open()){
		std::cout << "[FATAL] Log file open failed, no log will be saved!" << std::endl;
	}else{
		log_file << "==================== Program Start ====================" << std::endl;
	}
	log_inited = true; 
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

inline std::string GetFileName(const char* file){
	if(file == nullptr){
		return "";
	}
	std::string p(file);
	int id = -1;
	for (int i = p.size() - 1;i>=0;i--){
		if(p[i] == '/' || p[i] == '\\'){
			id = i;
			break;
		}
	}
	return p.substr(id + 1);
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

inline void Log (LogLevel level, const char* file, int line, const std::string& msg){
	std::lock_guard<std::mutex> lock(log_mtx);
	if(level < MIN_LOG_LEVEL){
		return;
	} 
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
	std::time_t now_ts = std::time(nullptr);
	std::tm local_tm = *std::localtime(&now_ts);
	std::string today = GetTodayDate(local_tm);
	std::string time_str = GetTimeStamp(local_tm);
	
	if(today != current_log_date){
		std::string newFile = GetDailyLogName(local_tm);
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
	std::string safe_msg = SanitizeMessage(msg);
	std::string file_name = file;
	if(full_path_on == false){
		file_name = GetFileName(file);
	}
	std::string logContent = "[" + time_str + "] [" + levelStr + "] " + file_name + ": " + std::to_string(line) + ": " + safe_msg; 
	if(log_file.is_open()){
		log_file << logContent << '\n';
		log_file.flush();
	}
} 

#define LOG_INFO(...)  do{ Log(LogLevel::INFO, __FILE__, __LINE__, FormatString(__VA_ARGS__)); }while(0)
#define LOG_WARN(...)  do{ Log(LogLevel::WARN, __FILE__, __LINE__, FormatString(__VA_ARGS__)); }while(0)
#define LOG_ERROR(...) do{ Log(LogLevel::ERROR, __FILE__, __LINE__, FormatString(__VA_ARGS__)); }while(0)
#define LOG_FATAL(...) do{ Log(LogLevel::FATAL, __FILE__, __LINE__, FormatString(__VA_ARGS__)); }while(0)
