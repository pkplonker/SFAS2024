#pragma once
#include <fstream>

#include "ISink.h"

class FileSink : public ISink
{
public:
    FileSink(std::string filePath);
    void Log(LogLevel level, const char* file, int line, const std::string message) override;
private:
    std::string path;
    std::ofstream logFile;
};
