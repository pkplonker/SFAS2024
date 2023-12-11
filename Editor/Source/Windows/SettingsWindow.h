#pragma once
#include <string>

#include "imgui.h"
#include "../EditorSettings.h"
class EditorSettingData;

class SettingsWindow
{
public:
    SettingsWindow();
    void Draw();

private:
    template <typename T>
    static void RenderSettingControl(const EditorSettingData& settingData)
    {
    }

    template <>
    static void RenderSettingControl<float>(const EditorSettingData& settingData)
    {
        try
        {
            ImGui::Text("%s", settingData.visableName.c_str());
            auto val = std::any_cast<float>(EditorSettings::GetSetting<float>(settingData.key));
            if (ImGui::DragFloat(("##" + settingData.key).c_str(), &val,1))
            {
                EditorSettings::Set<float>(settingData.key, val);
            }
        }
        catch (const std::exception& e)
        {
            Error(std::string("Bad cast for setting ") + e.what())
        }
    }

    template <>
    void RenderSettingControl<int>(const EditorSettingData& settingData)
    {
        try
        {
            ImGui::Text("%s", settingData.visableName.c_str());
            auto val = std::any_cast<int>(EditorSettings::GetSetting<int>(settingData.key));
            if (ImGui::DragInt(("##" + settingData.key).c_str(), &val, 1))
            {
                EditorSettings::Set<float>(settingData.key, val);
            }
        }
        catch (const std::exception& e)
        {
            Error(std::string("Bad cast for setting ") + e.what())
        }
    }

    template <>
    void RenderSettingControl<bool>(const EditorSettingData& settingData)
    {
        try
        {
            ImGui::Text("%s", settingData.visableName.c_str());
            auto val = std::any_cast<bool>(EditorSettings::GetSetting<bool>(settingData.key));
            if (ImGui::Checkbox(("##" + settingData.key).c_str(), &val))
            {
                EditorSettings::Set<float>(settingData.key, val);
            }
        }
        catch (const std::exception& e)
        {
            Error(std::string("Bad cast for setting ") + e.what())
        }
    }

    template <>
    void RenderSettingControl<std::string>(const EditorSettingData& settingData)
    {
        try
        {
            ImGui::Text("%s", settingData.visableName.c_str());

            auto val = std::any_cast<std::string>(EditorSettings::GetSetting<std::string>(settingData.key));

            char buffer[256];
            strncpy_s(buffer, val.c_str(), sizeof(buffer));
            buffer[sizeof(buffer) - 1] = '\0';

            if (ImGui::InputText(("##" + settingData.key).c_str(), buffer, sizeof(buffer)))
            {
                EditorSettings::Set<std::string>(settingData.key, std::string(buffer));
            }
        }
        catch (const std::exception& e)
        {
            Error(std::string("Bad cast for setting ") + e.what())
        }
    }

    bool testBool;
    float testfloat;
    int testInt;
    std::string testString;
};
