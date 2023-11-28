#pragma once
#include <string>
#include <windows.h>

class Helpers
{
public:
    static std::string WideStringToString(std::wstring wideText)
    {
        std::wstring wideStr(wideText);
        return std::string(wideStr.begin(), wideStr.end());
    }

    static std::wstring StringToWstring(const std::string& str)
    {
        int requiredSize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
        if (requiredSize == 0)
        {
            return L"";
        }

        std::wstring wideStr(requiredSize, L'\0');
        if (MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wideStr[0], requiredSize) == 0)
        {
            return L"";
        }

        return wideStr;
    }
};
