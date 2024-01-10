#include "SceneSettings.h"

#include "imgui.h"
#include "SceneManager.h"
#include "../ImGuiHelpers.h"
#include "Implementation/Scene.h"

void SceneSettings::Draw()
{
    ImGui::Begin("Scene Settings");
    if (const auto sharedScene = SceneManager::GetScene().lock())
    {
        ImGui::TextWrapped(sharedScene->GetPath().c_str());
        ImGui::Separator();
        ImGuiHelpers::UndoableColorEdit<Vec3>(
            [&]() { return sharedScene->GetAmbientLightColor(); },
            [sharedScene](Vec3 newColor) { sharedScene->SetAmbientLightColor(newColor); },
            "Ambient Light Color##LightColor",
            "Changed Ambient Light Color"
        );
    }
    ImGui::End();
}

std::string SceneSettings::GetName()
{
    return name;
}
