#pragma once
#include <string>

#define Trace(message) Debug::LogTrace(__FILE__,__LINE__, message);
#define Info(message) Debug::LogInfo(__FILE__,__LINE__, message);
#define Warning(message) Debug::LogWarning(__FILE__,__LINE__, message);
#define Error(message) Debug::LogError(__FILE__,__LINE__, message);

enum LogLevel
{
	Trace,
	Info,
	Warning,
	Error
};
class Debug
{
public:
	static std::string BeautifyLogLevel(LogLevel level);
	static void Log(LogLevel level, const char* file, int line, const std::string& message);
	static void LogTrace(const char* file, int line, const std::string& message);
	static void LogInfo(const char* file, int line, const std::string& message);
	static void LogWarning(const char* file, int line, const std::string& message);
	static void LogError(const char* file, int line, const std::string& message);

	static void SetLevel(LogLevel level);
	static LogLevel GetLevel();
	static bool GetShowLine();
	static void SetShowLine(bool value);
private:
	inline static bool showLineInfo = true;
	inline static LogLevel logLevel = Trace;
};
