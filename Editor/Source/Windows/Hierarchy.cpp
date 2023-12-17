#include "Hierarchy.h"

#include <memory>
#include <vector>

#include "imgui.h"
#include "SceneManager.h"
#include "../DefaultShapes.h"
#include "Engine/Implementation/GameObject.h"
#include "Engine/Implementation/GameObjectFactory.h"
#include "Engine/Implementation/Scene.h"

void Hierarchy::Draw()
{
    std::vector<std::shared_ptr<GameObject>> objectsToRemove;

    ImGui::Begin(HIERARCHY.c_str());
    if (auto sharedScene = SceneManager::GetScene().lock())
    {
        if (ImGui::IsMouseReleased(ImGuiMouseButton_Right) && ImGui::IsWindowHovered(ImGuiHoveredFlags_None))
        {
            ImGui::OpenPopup("ContextMenuWindow");
        }
        if (ImGui::BeginPopup("ContextMenuWindow"))
        {
            if (ImGui::BeginMenu("Add"))
            {
                if (ImGui::BeginMenu("Default Shapes"))
                {
                    if (ImGui::MenuItem("Cube"))
                    {
                        GameObjectFactory("New Cube").AddMeshRenderable(DefaultShapes::GetCubeMesh()).Build();
                    }
                    if (ImGui::MenuItem("Sphere"))
                    {
                        GameObjectFactory("New Sphere").AddMeshRenderable(DefaultShapes::GetSphereMesh()).Build();
                    }
                    if (ImGui::MenuItem("Plane"))
                    {
                        GameObjectFactory("New Plane").AddMeshRenderable(DefaultShapes::GetPlaneMesh()).Build();
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::MenuItem("Empty"))
                {
                    GameObjectFactory().Build();
                }
                if (ImGui::MenuItem("Empty Mesh"))
                {
                    GameObjectFactory("New Mesh").AddEmptyMeshRenderable().Build();
                }
                if (ImGui::MenuItem("Empty Sprite"))
                {
                    GameObjectFactory("New Sprite").AddEmptySpriteRenderable().Build();
                }
                
                
                if (ImGui::MenuItem("Camera"))
                {
                    GameObjectFactory("New Camera").AddPerspectiveCamera().Build();
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
            if (object == nullptr) continue;
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
                    renamingHelper.RequestRename(object);
                }

                ImGui::EndPopup();
            }
        }
        for (const auto object : objectsToRemove)
        {
            sharedScene->RemoveObject(object);
        }
        renamingHelper.DrawRenamePopup();

    }

    ImGui::End();
}

std::weak_ptr<GameObject> Hierarchy::GetSelectedObject()
{
    return selectedObject;
}
