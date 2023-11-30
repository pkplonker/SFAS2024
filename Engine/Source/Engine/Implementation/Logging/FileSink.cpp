#include "FileSink.h"

#include <fstream>
#include <iostream>


FileSink::FileSink(std::string filePath) : path(filePath)
{
    logFile.open(filePath, std::ios::app | std::ofstream::trunc);
    if (!logFile.is_open())
    {
        Error("Failed to open file for File sink")
    }
}

FileSink::~FileSink()
{
    if (logFile.is_open())
    {
        logFile.close();
    }
}

void FileSink::Log(LogLevel level, const char* file, int line, const std::string& message)
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
