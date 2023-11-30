#include "Debug.h"

#include <iostream>

std::string Debug::BeautifyLogLevel(LogLevel level)
{
    switch (level)
    {
    case Trace: return "[Trace]";
    case Info: return "[Info]";
    case Warning: return "[Warning]";
    case Error: return "[ERROR]";
    default: return "[]";
    }
}

void Debug::Log(LogLevel level, const char* file, int line, const std::string& message)
{
    if (level >= Debug::logLevel)
    {
        if (showLineInfo) std::cout << BeautifyLogLevel(level) << message << ": " << file << ":" << line << std::endl;
        else std::cout << message << std::endl;
    }
}

void Debug::LogTrace(const char* file, const int line, const std::string& message)
{
    Log(LogLevel::Trace, file, line, message);
}

void Debug::LogInfo(const char* file, int line, const std::string& message)
{
    Log(LogLevel::Info, file, line, message);
}

void Debug::LogWarning(const char* file, int line, const std::string& message)
{
    Log(LogLevel::Warning, file, line, message);
}

void Debug::LogError(const char* file, int line, const std::string& message)
{
    Log(LogLevel::Error, file, line, message);
}

void Debug::SetLevel(LogLevel level)
{
    logLevel = level;
}

LogLevel Debug::GetLevel()
{
    return logLevel;
}

bool Debug::GetShowLine()
{
    return showLineInfo;
}

void Debug::SetShowLine(bool value)
{
    showLineInfo = value;
}
