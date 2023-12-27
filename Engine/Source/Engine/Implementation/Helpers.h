#pragma once
#include <string>
#include <windows.h>

class GameObject;

class Helpers
{
public:
    static std::string WideStringToString(std::wstring wideText);

    static std::wstring StringToWstring(const std::string& str);
    static bool TryGetGuidFromString(std::string guidString, GUID& guid);
    static bool TryGetStringFromGuid(GUID guid, std::string& guidString);
};
