#include "Hierarchy.h"

#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#include "imgui.h"
#include "imgui_internal.h"
#include "SceneManager.h"
#include "../DefaultShapes.h"
#include "../ImGuiHelpers.h"
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
                CreateUndoableGameObject(
                    "New Cube",
                    [](GameObjectFactory& factory)
                    {
                        return factory.AddMeshRenderable(DefaultShapes::GetCubeMesh()).Build();
                    },
                    "Created cube"
                );


                CreateUndoableGameObject(
                    "New Sphere",
                    [](GameObjectFactory& factory)
                    {
                        return factory.AddMeshRenderable(DefaultShapes::GetSphereMesh()).Build();
                    },
                    "Created sphere"
                );

                CreateUndoableGameObject(
                    "New Plane",
                    [](GameObjectFactory& factory)
                    {
                        return factory.AddMeshRenderable(DefaultShapes::GetPlaneMesh()).Build();
                    },
                    "Created plane"
                );

                ImGui::EndMenu();
            }

            CreateUndoableGameObject(
                "New Empty",
                [](GameObjectFactory& factory)
                {
                    return factory.Build();
                },
                "Created empty object"
            );

            CreateUndoableGameObject(
                "New Mesh",
                [](GameObjectFactory& factory)
                {
                    return factory.AddEmptyMeshRenderable().Build();
                },
                "Created empty mesh"
            );

            CreateUndoableGameObject(
                "New Sprite",
                [](GameObjectFactory& factory)
                {
                    return factory.AddEmptySpriteRenderable().Build();
                },
                "Created empty sprite"
            );

            CreateUndoableGameObject(
                "New Camera",
                [](GameObjectFactory& factory)
                {
                    return factory.AddPerspectiveCamera().Build();
                },
                "Created camera"
            );
            if (ImGui::MenuItem("Light"))
            {
            }

            ImGui::EndMenu();
        }

        ImGui::EndPopup();
    }
}

void Hierarchy::CreateUndoableGameObject(
    const std::string& name,
    std::function<std::shared_ptr<GameObject>(GameObjectFactory&)> createObjectFunc,
    const std::string& actionDescription)
{
    auto guidPtr = std::make_shared<std::string>();

    ImGuiHelpers::UndoableMenuItemAction(
        name.c_str(),
        [guidPtr, createObjectFunc, name]()
        {
            if (const auto scene = SceneManager::GetScene().lock())
            {
                GameObjectFactory factory(name);
                auto createdObject = createObjectFunc(factory);
                *guidPtr = createdObject->GetGUID();
                scene->AddObject(createdObject);
            }
        },
        [guidPtr]()
        {
            if (const auto scene = SceneManager::GetScene().lock())
            {
                if (!guidPtr->empty())
                {
                    scene->RemoveObject(*guidPtr);
                }
            }
        },
        actionDescription
    );
}


void Hierarchy::SetSelectedObject(std::shared_ptr<GameObject> object)
{
    std::weak_ptr<GameObject> previousObject = selectedObject;
    UndoManager::Execute(Memento([this, object]()
                                 {
                                     selectedObject = object;
                                 }, [this, previousObject]()
                                 {
                                     selectedObject = previousObject;
                                 }, "New selection"));
}

void Hierarchy::ProcessChildren(std::vector<std::shared_ptr<GameObject>>& objectsToRemove, ImGuiTreeNodeFlags baseFlags,
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
                bool isSelected = (selectedObject.lock() == object);
                if (isSelected)
                {
                    localFlags |= ImGuiTreeNodeFlags_Selected;
                    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.1764705926179886f, 0.3490196168422699f,
                                                                  0.5764706134796143f,
                                                                  0.8619999885559082f));
                }

                bool nodeOpen = ImGui::TreeNodeEx(nodeLabel.c_str(), localFlags);
                if (isSelected)
                {
                    ImGui::PopStyleColor(1);
                }

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
                    SetSelectedObject(object);
                }

                if (ImGui::BeginPopupContextItem(("ObjectContextMenu##" + object->GetGUID()).c_str()))
                {
                    //SetSelectedObject(object);
                    ImGuiHelpers::UndoableMenuItemAction(
                        "Delete",
                        [&objectsToRemove, object]()
                        {
                            objectsToRemove.emplace_back(object);
                        },
                        [object]()
                        {
                            if (const auto scene = SceneManager::GetScene().lock())
                            {
                                scene->AddObject(object);

                                //setting to scene as default and then attempting to set to old
                                const auto oldParent = object->Transform()->parent;
                                object->Transform()->SetParentWeak(scene);
                                object->Transform()->SetParentWeak(oldParent);
                                // need to add back to renderables
                            }
                        },
                        "Deleting object"
                    );

                    std::string originalName = object->Name;
                    ImGuiHelpers::UndoableMenuItemAction(
                        "Rename",
                        [this, object]()
                        {
                            this->renamingHelper.RequestRename(object);
                        },
                        [object, originalName]()
                        {
                            object->Name = originalName;
                        },
                        "Renaming object"
                    );

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
            if (selectedObject.lock()->GetGUID() == object->GetGUID())
            {
                selectedObject = std::make_shared<GameObject>();
            }
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
