#include "BufferSink.h"

#include <iostream>

BufferSink::BufferSink(int size)
{
    maxSize = size;
    Debug::RegisterSink(this);
}

void BufferSink::Log(LogLevel level, const char* file, int line, const std::string& message)
{
    if (buffer.size() >= maxSize)
    {
        buffer.pop_back();
    }
    if (showLineInfo) buffer.push_front(BeautifyLogLevel(level) + message + ": " + file + ":" + std::to_string(line));
    else buffer.push_front(BeautifyLogLevel(level) + message);
}

std::pmr::deque<std::string> BufferSink::GetBuffer()
{
    return buffer;
}
