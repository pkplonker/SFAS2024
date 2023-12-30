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
        ImGui::SameLine();
        renamingHelper.DrawRenamePopup();
        ImGui::SameLine();

        ImGui::Text(gameobject->GetGUID().c_str());
        auto state = gameobject->GetIsEnabled();
        ImGui::SameLine();

        if (ImGuiHelpers::ButtonWithState(state ? "Disable" : "Enable", ImVec2(50, 20), state,false,ImVec4(0.8f,0,0,1)))
        {
            gameobject->SetIsEnabled(!state);
        }

        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::BeginPopupContextItem("TransformContextMenu"))
            {
                ImGuiHelpers::UndoableMenuItemValue<Vec3>(
                    "Reset Position",
                    [gameobject]() { return gameobject->Transform()->Position; },
                    [gameobject](const Vec3& newVal) { gameobject->Transform()->Position = newVal; },
                    Vec3::Zero(),
                    "Reset GameObject Position"
                );
                ImGuiHelpers::UndoableMenuItemValue<Vec3>(
                    "Reset Rotation",
                    [gameobject]() { return gameobject->Transform()->Rotation; },
                    [gameobject](const Vec3& newVal) { gameobject->Transform()->Rotation = newVal; },
                    Vec3::Zero(),
                    "Reset GameObject rotation"
                );
                ImGuiHelpers::UndoableMenuItemValue<Vec3>(
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
                    auto comp = std::make_shared<std::shared_ptr<IComponent>>(nullptr);

                    UndoManager::Execute(Memento(
                        [this, comp, component]()
                        {
                            if (const auto gameobject = this->hierarchy->GetSelectedObject().lock())
                            {
                                *comp = ComponentRegistry::CreateComponent(component, gameobject);
                                gameobject->AddComponent(*comp);
                            }
                        },
                        [this, comp]()
                        {
                            if (comp)
                            {
                                if (auto gameobject = this->hierarchy->GetSelectedObject().lock())
                                {
                                    gameobject->RemoveComponent(*comp);
                                }
                            }
                        },
                        "Adding component"
                    ));

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
