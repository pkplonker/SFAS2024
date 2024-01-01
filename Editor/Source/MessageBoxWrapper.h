#pragma once

#include <Windows.h>
#include <string>

class MessageBoxWrapper
{
public:
    static void ShowWarning(const std::string& message, const std::string& title)
    {
        Show(message, title, MB_ICONEXCLAMATION | MB_OK);
    }

    static void ShowInfo(const std::string& message, const std::string& title)
    {
        Show(message, title, MB_ICONASTERISK | MB_OK);
    }

    static bool ShowSaveConfirmation(std::string caption = "Save Scene",
                                     std::string text = "Do you want to save the current scene?")
    {
        const int response = MessageBox(nullptr,
                                        text.c_str(),
                                        caption.c_str(),
                                        MB_YESNOCANCEL | MB_ICONWARNING);

        return response == IDYES
                   ? true
                   : false;
    }

private :
    static void Show(const std::string& message, const std::string& title, UINT type)
    {
        MessageBox(nullptr, message.c_str(), title.c_str(), type);
        Warning(message)
    }
};
