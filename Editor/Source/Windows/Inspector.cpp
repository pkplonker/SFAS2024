#include "Inspector.h"

#include "ComponentRegistry.h"
#include "Hierarchy.h"
#include "imgui.h"
#include "../ImGuiHelpers.h"
#include "Engine/Implementation/GameObject.h"
#include "Logging/Debug.h"

Inspector::Inspector(std::shared_ptr<Hierarchy> hierarchy) : hierarchy(hierarchy),
                                                             factory(std::make_unique<ComponentDrawerFactory>())
{
}

void Inspector::Draw()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12.0f, 12.0f));

    ImGui::Begin(INSPECTOR.c_str());

    if (const auto gameobject = hierarchy->GetSelectedObject().lock())
    {
        ImGui::Text(gameobject->Name.c_str());
        ImGui::SameLine();
        if (ImGui::Button("...##renamebutton"))
        {
            renamingHelper.RequestRename(gameobject);
        }
        renamingHelper.DrawRenamePopup();
        ImGui::SameLine();

        ImGui::Text(gameobject->GetGUID().c_str());

        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::BeginPopupContextItem("TransformContextMenu"))
            {
                ImGuiHelpers::UndoableMenuItem<Vec3>(
                    "Reset Position",
                    [gameobject]() { return gameobject->Transform()->Position; },
                    [gameobject](const Vec3& newVal) { gameobject->Transform()->Position = newVal; },
                    Vec3::Zero(),
                    "Reset GameObject Position"
                );
                ImGuiHelpers::UndoableMenuItem<Vec3>(
                    "Reset Rotation",
                    [gameobject]() { return gameobject->Transform()->Rotation; },
                    [gameobject](const Vec3& newVal) { gameobject->Transform()->Rotation = newVal; },
                    Vec3::Zero(),
                    "Reset GameObject rotation"
                );
                ImGuiHelpers::UndoableMenuItem<Vec3>(
                    "Reset Scale",
                    [gameobject]() { return gameobject->Transform()->Scale; },
                    [gameobject](const Vec3& newVal) { gameobject->Transform()->Scale = newVal; },
                    Vec3::Zero(),
                    "Reset GameObject scale"
                );
                ImGui::EndPopup();
            }
            ImGuiHelpers::DrawVector("Position", gameobject->Transform()->Position);
            ImGuiHelpers::DrawVector("Rotation", gameobject->Transform()->Rotation);
            ImGuiHelpers::DrawVector("Scale", gameobject->Transform()->Scale);
        }


        for (auto component : gameobject->GetComponents())
        {
            auto drawer = factory->CreateDrawer(component);
            if (drawer != nullptr)
            {
                drawer->Draw();
            }
        }
        ImGuiHelpers::SpacedSeperator();


        if (ImGuiHelpers::ButtonCenteredOnLine("Add Component"))
        {
            Trace("Pressed")
            ImGui::OpenPopup("AddComponentPopup");
        }
        if (ImGui::BeginPopup("AddComponentPopup"))
        {
            std::vector<std::string> components = ComponentRegistry::GetComponentTypes();

            for (const auto& component : components)
            {
                if (ImGui::Selectable(component.c_str()))
                {
                    Trace("Adding " + component + " to GameObject");
                    auto comp = ComponentRegistry::CreateComponent(component, gameobject);
                    gameobject->AddComponent(comp);
                    ImGui::CloseCurrentPopup();
                    break;
                }
            }

            ImGui::EndPopup();
        }
    }
    else
    {
        ImGui::Text("Select an object to inspect.");
    }
    ImGui::PopStyleVar(1);
    ImGui::End();
}
