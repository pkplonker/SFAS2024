#include "Hierarchy.h"

#include <memory>
#include <vector>

#include "imgui.h"
#include "Engine/Implementation/GameObject.h"
#include "Engine/Implementation/GameObjectFactory.h"
#include "Engine/Implementation/Scene.h"

Hierarchy::Hierarchy(const std::weak_ptr<Scene>& scene) : scene(scene)
{
}

void Hierarchy::Draw()
{
    std::vector<std::shared_ptr<GameObject>> objectsToRemove;

    ImGui::Begin(HIERARCHY.c_str());
    if (auto sharedScene = scene.lock())
    {
        if (ImGui::IsMouseReleased(ImGuiMouseButton_Right) && ImGui::IsWindowHovered(ImGuiHoveredFlags_None))
        {
            ImGui::OpenPopup("ContextMenuWindow");
        }
        if ( ImGui::BeginPopup("ContextMenuWindow"))
        {
            if (ImGui::BeginMenu("Add"))
            {
                if (ImGui::MenuItem("Empty"))
                {
                    GameObjectFactory(sharedScene, "New GameObject").Build();
                }
                if (ImGui::MenuItem("Camera"))
                {
                    GameObjectFactory(sharedScene, "New Camera").AddPerspectiveCamera().Build();
                }
                if (ImGui::MenuItem("Light"))
                {
                }

                ImGui::EndMenu();
            }


            ImGui::EndPopup();
        }

        for (const auto& object : sharedScene->GetObjects())
        {
            std::string label = object->Name;
            bool is_selected = (selectedObject.lock() == object);

            if (ImGui::Selectable(label.c_str(), is_selected))
            {
                selectedObject = object;
            }


            if (ImGui::BeginPopupContextItem(
                ("ObjectContextMenu" + std::to_string(reinterpret_cast<std::uintptr_t>(object.get()))).c_str()))
            // use the memory address as a unique identifer
            {
                selectedObject = object;
                if (ImGui::MenuItem("Delete"))
                {
                    objectsToRemove.emplace_back(object);
                }
                if (ImGui::MenuItem("Rename"))
                {
                }

                ImGui::EndPopup();
            }
        }
        for (const auto object : objectsToRemove)
        {
            sharedScene->RemoveObject(object);
        }
    }


    ImGui::End();
}

std::weak_ptr<GameObject> Hierarchy::GetSelectedObject()
{
    return selectedObject;
}
