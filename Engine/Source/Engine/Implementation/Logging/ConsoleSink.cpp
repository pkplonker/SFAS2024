#include "ConsoleSink.h"

#include <iostream>
#include <sstream>

ConsoleSink::ConsoleSink()
{
    Debug::RegisterSink(this);
}

void ConsoleSink::Log(LogLevel level, const char* file, int line, const std::string message)
{
    std::ostringstream logStream;

    logStream << BeautifyLogLevel(level) << message;

    if (showLineInfo) {
        logStream << ": " << file << ":" << line;
    }

    logStream << std::endl;

    std::cout << logStream.str();
}
