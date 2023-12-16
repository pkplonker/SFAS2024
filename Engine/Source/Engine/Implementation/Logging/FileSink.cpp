#include "FileSink.h"

#include <fstream>
#include <iostream>


FileSink::FileSink(std::string filePath) : path(filePath)
{
    logFile.open(filePath, std::ofstream::trunc);
    if (!logFile.is_open())
    {
        Error("Failed to open file for File sink")
    }
    Debug::RegisterSink(this);
}

FileSink::~FileSink()
{
    FlushBuffer();
}

void FileSink::Log(LogLevel level, const char* file, int line, const std::string message)
{
    if (showLineInfo)
    {
        buffer << BeautifyLogLevel(level) << ": " << message << ": " << file << ":" << line << "\n";
    }
    else
    {
        buffer << message << "\n";
    }

    if (buffer.str().size() >= bufferSizeLimit)
    {
        FlushBuffer();
    }
}

void FileSink::FlushBuffer()
{
    if (logFile.is_open() && !buffer.str().empty())
    {
        logFile << buffer.str();
        buffer.str("");
        buffer.clear();
    }
}
