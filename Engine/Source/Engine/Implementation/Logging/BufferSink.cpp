#include "BufferSink.h"

#include <iostream>

BufferSink::BufferSink(int size)
{
    maxSize = size;
    Debug::RegisterSink(this);
}

void BufferSink::Log(LogLevel level, const char* file, int line, const std::string message)
{
    if (buffer.size() >= maxSize)
    {
        buffer.pop_back();
    }
    buffer.push_front(LogMessageData(level, file, line, message));
    isDirty = true;
}

std::pmr::deque<LogMessageData> BufferSink::GetBuffer()
{
    return buffer;
}

void BufferSink::Clear()
{
    buffer.clear();
}

bool BufferSink::IsDirty()
{
    return isDirty;
}
