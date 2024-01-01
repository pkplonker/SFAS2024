#include "SceneSettings.h"

#include "imgui.h"
#include "SceneManager.h"
#include "Implementation/Scene.h"

void SceneSettings::Draw()
{
    ImGui::Begin("Scene Settings");
    if (const auto sharedScene = SceneManager::GetScene().lock())
    {
        ImGui::TextWrapped(sharedScene->GetPath().c_str());
    }
    ImGui::End();
}

std::string SceneSettings::GetName()
{
    return name;
}
