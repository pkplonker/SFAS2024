#pragma once
#include <xstring>

class FileDialog
{
public:
    static std::string OpenFileDialog(std::string fileFilter);
    static std::string OpenFileDialog();
    static std::string SaveFileDialog();
};
