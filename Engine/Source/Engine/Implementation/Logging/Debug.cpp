#include "Debug.h"

#include <iostream>

#include "ISink.h"


void Debug::Log(LogLevel level, const char* file, int line, const std::string& message)
{
    if (level >= Debug::logLevel)
    {
        for (auto sink : sinks)
        {
            sink->Log(level,file, line, message);
        }
    }
}

Debug::~Debug()
{
    for (auto sink : sinks)
    {
        delete sink;
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

ISink* Debug::RegisterSink(ISink* sink)
{
    sinks.emplace(sink);
    return sink;
}

void Debug::DeregisterSink(ISink* sink)
{
    auto it = std::find(sinks.begin(), sinks.end(), sink);
    if (it != sinks.end())
    {
        sinks.erase(it);
    }
}
