#include "ConsoleSink.h"

#include <iostream>

ConsoleSink::ConsoleSink()
{
    Debug::RegisterSink(this);
}

void ConsoleSink::Log(LogLevel level, const char* file, int line, const std::string& message)
{
    if (showLineInfo) std::cout << BeautifyLogLevel(level) << message << ": " << file << ":" << line << std::endl;
    else std::cout << BeautifyLogLevel(level) << message << std::endl;
}
