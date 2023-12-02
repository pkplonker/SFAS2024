#pragma once
#include "ISink.h"

class ConsoleSink : public ISink
{
public:
    ConsoleSink();
    void Log(LogLevel level, const char* file, int line, const std::string& message) override;
    
};
