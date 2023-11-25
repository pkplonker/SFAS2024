#pragma once
#include <string>

class Helpers
{
public:
    static std::string WideStringToString(std::wstring wideText)
    {
        std::wstring wideStr(wideText);
        return std::string (wideStr.begin(), wideStr.end());
    }
};