#pragma once
#include <filesystem>
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
    static std::filesystem::path GetExecutablePath();
    static std::wstring ToAbsolutePath(const std::wstring& wstring);
    static std::string ToAbsolutePath(const std::string& inputPath);
    static std::wstring ToRelativePath(const std::wstring& inputPath);
    static std::string ToRelativePath(const std::string& inputPath);
};
