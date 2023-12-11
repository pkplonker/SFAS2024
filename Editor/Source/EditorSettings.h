#pragma once
#include <any>
#include <assert.h>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "Logging/Debug.h"

enum EditorSettingType
{
    stringType,
    boolType,
    intType,
    floatType,
};

class EditorSettingData
{
public:
    EditorSettingData(std::string name, std::string key, EditorSettingType type) : visableName(name), key(key),
        type(type)
    {
    }

    std::string visableName;
    std::string key;
    EditorSettingType type;
};

struct EditorSettingDataHash
{
    size_t operator()(const EditorSettingData& data) const
    {
        return std::hash<std::string>()(data.key);
    }
};

struct EditorSettingDataEqual
{
    bool operator()(const EditorSettingData& lhs, const EditorSettingData& rhs) const
    {
        return lhs.key == rhs.key;
    }
};

class EditorSettings
{
public:
    template <typename T>
    static void ValidateInput()
    {
        static_assert(
            std::is_same<T, int>::value || std::is_same<T, float>::value || std::is_same<T, bool>::value || std::is_same
            <T, std::string>::value,
            "EditorSettings::Get only supports int, float, string and bool types.");
    }

    template <typename T>
    static void Set(const std::string& key, const T& value)
    {
        ValidateInput<T>();

        LoadSettings();
        std::ostringstream oss;
        oss << value;
        settingsMap[key] = oss.str();
        SaveSettings();
    }

    template <typename T>
    static void DetermineType(EditorSettingType& type)
    {
        if constexpr (std::is_same_v<T, int>)
        {
            type = EditorSettingType::intType;
        }
        else if constexpr (std::is_same_v<T, float>)
        {
            type = EditorSettingType::floatType;
        }
        else if constexpr (std::is_same_v<T, std::string>)
        {
            type = EditorSettingType::stringType;
        }
        else if constexpr (std::is_same_v<T, bool>)
        {
            type = EditorSettingType::boolType;
        }
        else
        {
            Error("What?")
            assert(false);
        }
    }

    template <typename T>
    static void ExposeSetting(const std::string& key, std::string visibleName, std::string categoryName, T value)
    {
        if (visibleName != "" && categoryName != "")
        {
            EditorSettingType type;
            DetermineType<T>(type);
            exposedSettings[categoryName].emplace(EditorSettingData(visibleName, key, type));
        }
    }

    template <typename T>
    static T Get(const std::string& key, const T defaultValue, std::string visibleName = "",
                 std::string categoryName = "")
    {
        ValidateInput<T>();

        LoadSettings();
        auto it = settingsMap.find(key);
        if (it != settingsMap.end())
        {
            std::stringstream ss(it->second);
            T value;
            ss >> value;
            if (!ss.fail())
            {
                ExposeSetting<T>(key, visibleName, categoryName, value);
                return value;
            }
        }
        Set(key, defaultValue);
        ExposeSetting<T>(key, visibleName, categoryName, defaultValue);
        return defaultValue;
    }

    template <typename T>
    static T GetSetting(const std::string& key)
    {
        ValidateInput<T>();

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
        if constexpr (std::is_pointer<T>::value)
        {
            return nullptr;
        }
        if constexpr (std::is_fundamental<T>::value)
        {
            return T(0);
        }

        return T();
    }

    static std::unordered_map<std::string, std::unordered_set<
                                  EditorSettingData, EditorSettingDataHash, EditorSettingDataEqual>>
    GetExposedSettings()
    {
        return exposedSettings;
    }

    static void SaveSettings();

private:
    static inline std::unordered_map<std::string, std::string> settingsMap = {};
    static inline std::string settingsFile =
        "S:/Users/pkplo/OneDrive/Documents/C++/SFAS2024/Editor/Resource/settings.settings";
    static inline bool init = false;
    static inline std::unordered_map<std::string, std::unordered_set<
                                         EditorSettingData, EditorSettingDataHash, EditorSettingDataEqual>>
    exposedSettings;
    static void LoadSettings();
};
