#include "EditorSettings.h"

#include <filesystem>
#include <fstream>

#include "Logging/Debug.h"

void EditorSettings::SaveSettings()
{
    Trace("Saving to " + settingsFile)
    std::ofstream file(settingsFile);
    if (file)
    {
        for (const auto& pair : settingsMap)
        {
            file << pair.first << "=" << pair.second << std::endl;
        }
    }else
    {
        Error("Unable to save editor settings")
    }
}

void EditorSettings::LoadSettings()
{
    Trace(std::filesystem::current_path().string())
    if (!init)
    {
        std::ifstream file(settingsFile);
        std::string line;
        while (std::getline(file, line))
        {
            std::istringstream iss(line);
            std::string key, value;
            if (std::getline(iss, key, '=') && std::getline(iss, value))
            {
                settingsMap[key] = value;
            }
        }
        init = true;
    }
}
