#pragma once
#include <fstream>
#include <sstream>

#include "ISink.h"

class FileSink : public ISink
{
public:
    FileSink(std::string filePath);
    ~FileSink() override;
    void Log(LogLevel level, const char* file, int line, const std::string message) override;

private:
    std::string path;
    std::ofstream logFile;
    std::ostringstream buffer;
    const int bufferSizeLimit = 1024 * 10;

    void FlushBuffer();
};
