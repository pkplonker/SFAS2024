#include "Helpers.h"

#include <memory>

#include "GameObject.h"
#include "Logging/Debug.h"

std::string Helpers::WideStringToString(std::wstring wideText)
{
    std::wstring wideStr(wideText);
    return std::string(wideStr.begin(), wideStr.end());
}

std::wstring Helpers::StringToWstring(const std::string& str)
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

bool Helpers::TryGetGuidFromString(std::string guidString, GUID& guid)
{
    auto guidWideString = StringToWstring(guidString);

    HRESULT hr = CLSIDFromString(guidWideString.c_str(), &guid);
    if (!SUCCEEDED(hr))
    {
        Warning("Failed to convert serialized string to GUID");
        return false;
    }
    return true;
}

bool Helpers::TryGetStringFromGuid(GUID guid, std::string& guidString)
{
    OLECHAR* olechar;
    HRESULT hr = StringFromCLSID(guid, &olechar);
    bool result = false;
    if (SUCCEEDED(hr))
    {
        guidString = WideStringToString(olechar);
        result = true;
    }
    CoTaskMemFree(olechar);
    return result;
}
