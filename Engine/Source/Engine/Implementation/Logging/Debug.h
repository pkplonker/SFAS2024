#pragma once
#include <string>
#include <unordered_set>

#define Trace(message) Debug::LogTrace(__FILE__,__LINE__, message);
#define Info(message) Debug::LogInfo(__FILE__,__LINE__, message);
#define Warning(message) Debug::LogWarning(__FILE__,__LINE__, message);
#define Error(message) Debug::LogError(__FILE__,__LINE__, message);

class ISink;

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
    static void Log(LogLevel level, const char* file, int line, const std::string& message);
    ~Debug();
    static void LogTrace(const char* file, int line, const std::string& message);
    static void LogInfo(const char* file, int line, const std::string& message);
    static void LogWarning(const char* file, int line, const std::string& message);
    static void LogError(const char* file, int line, const std::string& message);

    static void SetLevel(LogLevel level);
    static LogLevel GetLevel();
    static ISink* RegisterSink(ISink* sink);
    static void DeregisterSink(ISink* sink);

private:
    inline static LogLevel logLevel = Trace;
    inline static std::pmr::unordered_set<ISink*> sinks;
};
