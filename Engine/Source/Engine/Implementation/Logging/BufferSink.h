#pragma once
#include <deque>

#include "ISink.h"

struct LogMessageData
{
    LogMessageData(LogLevel level, const char* file, int line, const std::string message) : level(level), file(file),
        line(line), message(message)
    {
    }

    LogLevel level;
    const char* file;
    int line;
    const std::string message;
};

class BufferSink : public ISink
{
public:
    BufferSink(int size);
    void Log(LogLevel level, const char* file, int line, const std::string message) override;
    std::pmr::deque<LogMessageData> GetBuffer();
    void Clear();

private:
    std::pmr::deque<LogMessageData> buffer;
    unsigned int maxSize;
};
