#pragma once
#include <deque>

#include "ISink.h"

class BufferSink : public ISink
{
public:
    BufferSink(int size);
    void Log(LogLevel level, const char* file, int line, const std::string& message) override;
    std::pmr::deque<std::string> GetBuffer();
private:
    std::pmr::deque<std::string> buffer;
    unsigned int maxSize;
};
