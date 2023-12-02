#include "FileSink.h"

#include <fstream>
#include <iostream>


FileSink::FileSink(std::string filePath) : path(filePath)
{
    logFile.open(filePath, std::ofstream::trunc);
    if (!logFile.is_open())
    {
        Error("Failed to open file for File sink")
        return;
    }
    Debug::RegisterSink(this);
}

FileSink::~FileSink()
{
    logFile.flush();
}


void FileSink::Log(LogLevel level, const char* file, int line, const std::string message)
{
    if (logFile.is_open())
    {
        if (showLineInfo)
        {
            logFile << BeautifyLogLevel(level) << ": " << message << ": " << file << ":" << line << std::endl;
        }
        else
        {
            logFile << message << std::endl;
        }
    }
}
