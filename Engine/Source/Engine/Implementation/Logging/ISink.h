#pragma once
#include "Debug.h"

class ISink
{
public:
    virtual ~ISink() = default;
    virtual void Log(LogLevel level, const char* file, int line, const std::string message) =0;
    bool GetShowLine();
    void SetShowLine(bool value);
    static std::string BeautifyLogLevel(LogLevel level);
    virtual bool IsDirty() { return true; }

    virtual void ClearDirty()
    {
    }

protected:
    bool showLineInfo = true;
};
