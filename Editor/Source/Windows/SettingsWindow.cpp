#include "SettingsWindow.h"
#include "../EditorSettings.h"
#include "imgui.h"

SettingsWindow::SettingsWindow()
{
   
}

void SettingsWindow::Draw()
{
    testBool = EditorSettings::Get("TEST_BOOL", true, "Test bool", "Test");
    testfloat = EditorSettings::Get("TEST_FLOAT", 666.0f, "Test Float", "Test");
    testInt = EditorSettings::Get("TEST_INT", 999, "Test int", "Test");
    testString = EditorSettings::Get("TEST_STRING", std::string("Test"), "Test String", "Test");
    if (ImGui::BeginPopupModal("Settings"))
    {
        auto exposedSettings = EditorSettings::GetExposedSettings();

        if (ImGui::BeginTabBar("SettingsCategories"))
        {
            for (const auto& category : exposedSettings)
            {
                if (ImGui::BeginTabItem(category.first.c_str()))
                {
                    for (const auto& settingData : category.second)
                    {
                        switch (settingData.type)
                        {
                        case stringType:
                            RenderSettingControl<std::string>(settingData);
                            break;
                        case boolType:
                            RenderSettingControl<bool>(settingData);
                            break;
                        case intType:
                            RenderSettingControl<int>(settingData);
                            break;
                        case floatType:
                            RenderSettingControl<float>(settingData);
                            break;
                        default:
                            assert(false);
                            break;
                        }
                    }
                    ImGui::EndTabItem();
                }
            }
            ImGui::EndTabBar();
        }

        if (ImGui::Button("Close"))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}
