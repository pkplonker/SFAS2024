#include "Hierarchy.h"

#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#include "imgui.h"
#include "imgui_internal.h"
#include "SceneManager.h"
#include "../DefaultShapes.h"
#include "Engine/Implementation/GameObject.h"
#include "Engine/Implementation/GameObjectFactory.h"
#include "Engine/Implementation/Scene.h"
#include "Logging/Debug.h"

void Hierarchy::HandleContextMenu(const char* contextMenuName)
{
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Right) && ImGui::IsWindowHovered(ImGuiHoveredFlags_None))
    {
        ImGui::OpenPopup(contextMenuName);
    }
    if (ImGui::BeginPopup(contextMenuName))
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
}

void Hierarchy::ProcessChildren(std::vector<std::shared_ptr<GameObject>> objectsToRemove, ImGuiTreeNodeFlags baseFlags,
                                std::set<std::weak_ptr<Transform>, Transform::TransformCompare> children)
{
    for (const auto& transform : children)
    {
        try
        {
            if (auto object = transform.lock()->gameobject.lock())
            {
                std::string label = object->Name;
                int localFlags = baseFlags;

                std::string nodeLabel = label + "##" + object->GetGUID();
                if (!object->Transform()->HasChildren())
                {
                    localFlags |= ImGuiTreeNodeFlags_Leaf;
                }


                bool nodeOpen = ImGui::TreeNodeEx(nodeLabel.c_str(), localFlags);

                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
                {
                    ImGui::SetDragDropPayload(DragDropPayloadID, object->GetGUID().c_str(),
                                              object->GetGUID().size() + 1);
                    ImGui::EndDragDropSource();
                }
                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(DragDropPayloadID))
                    {
                        std::string payloadGUID = static_cast<const char*>(payload->Data);
                        if (payloadGUID != object->GetGUID())
                        {
                            if (const auto scene = SceneManager::GetScene().lock())
                            {
                                std::weak_ptr<GameObject> foundObject;
                                if (scene->TryFindObject(payloadGUID, foundObject))
                                {
                                    foundObject.lock()->Transform()->SetParent(object->Transform());
                                }
                            }
                        }
                    }
                    ImGui::EndDragDropTarget();
                }
                if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                {
                    selectedObject = object;
                }

                if (ImGui::BeginPopupContextItem(("ObjectContextMenu##" + object->GetGUID()).c_str()))
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

                if (nodeOpen)
                {
                    ProcessChildren(objectsToRemove, baseFlags, transform.lock()->children);
                    ImGui::TreePop();
                }
            }
        }
        catch (std::exception e)
        {
        }
    }
}


void Hierarchy::Draw()
{
    std::vector<std::shared_ptr<GameObject>> objectsToRemove;

    ImGui::Begin(HIERARCHY.c_str());
    const auto window = ImGui::GetCurrentWindow();
    if (ImGui::BeginDragDropTargetCustom(window->ContentRegionRect, window->ID))
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(DragDropPayloadID))
        {
            std::string payloadGUID = static_cast<const char*>(payload->Data);
            if (const auto scene = SceneManager::GetScene().lock())
            {
                std::weak_ptr<GameObject> foundObject;
                if (scene->TryFindObject(payloadGUID, foundObject))
                {
                    auto draggedObject = foundObject.lock();
                    if (draggedObject)
                    {
                        draggedObject->Transform()->SetParent(scene);
                    }
                }
            }
        }
        ImGui::EndDragDropTarget();
    }

    auto contextMenuName = "ContextMenuWindow";
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow |
        ImGuiTreeNodeFlags_SpanAvailWidth;

    if (auto sharedScene = SceneManager::GetScene().lock())
    {
        HandleContextMenu(contextMenuName);
        auto children = sharedScene->children;
        ProcessChildren(objectsToRemove, flags, children);
        for (const auto& object : objectsToRemove)
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
