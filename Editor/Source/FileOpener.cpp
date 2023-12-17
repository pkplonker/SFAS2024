#include "FileOpener.h"

#include <sstream>


#include "Helpers.h"
#include "Logging/Debug.h"

void FileOpener::Open(const std::wstring& filePath)
{
    Open(Helpers::WideStringToString(filePath));
}

void FileOpener::Open(const std::string& path)
{
    try
    {
        ShellExecute(nullptr, "open", path.c_str(), nullptr, nullptr, SW_SHOW);
    }
    catch (std::exception& e)
    {
        std::ostringstream errStream;
        errStream << "Failed to open " << path << " " << e.what();

        std::string err = errStream.str();
        Warning(err)
    }
}
