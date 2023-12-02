#pragma once
#include <sstream>
#include <string>
#include <unordered_map>

class EditorSettings
{
public:
    template <typename T>
    static void Set(const std::string& key, const T& value)
    {
        LoadSettings();
        std::ostringstream oss;
        oss << value;
        settingsMap[key] = oss.str();
        SaveSettings();
    }

    template <typename T>
    static T Get(const std::string& key, const T& defaultValue)
    {
        LoadSettings();
        auto it = settingsMap.find(key);
        if (it != settingsMap.end())
        {
            std::stringstream ss(it->second);
            T value;
            ss >> value;
            if (!ss.fail())
            {
                return value;
            }
        }
        return defaultValue;
    }

    static void SaveSettings();

private:
    static inline std::unordered_map<std::string, std::string> settingsMap = {};
    static inline std::string settingsFile =
        "S:/Users/pkplo/OneDrive/Documents/C++/SFAS2024/Editor/Resource/settings.settings";
    static inline bool init = false;

    static void LoadSettings();
};
