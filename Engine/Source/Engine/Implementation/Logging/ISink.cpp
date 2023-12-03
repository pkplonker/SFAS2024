#include "ISink.h"


bool ISink::GetShowLine()
{
    return showLineInfo;
}

void ISink::SetShowLine(bool value)
{
    showLineInfo = value;
}

std::string ISink::BeautifyLogLevel(LogLevel level)
{
    switch (level)
    {
    case Trace: return "[Trace]";
    case Info: return "[Info]";
    case Warning: return "[Warning]";
    case Error: return "[ERROR]";
    default: return "[]";
    }
}
