#pragma once
#include <xstring>

class FileOpener
{
public:
    static void Open(const std::wstring& filePath);
    static void Open(const std::string& filePath);
};
