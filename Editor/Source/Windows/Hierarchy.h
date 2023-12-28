#pragma once
#include <memory>

#include "ComponentDrawerFactory.h"
#include "EditorWindow.h"
#include "GameObjectFactory.h"
#include "RenamingHelper.h"
class GameObject;
class Scene;
const std::string HIERARCHY = "Hierarchy";
const int RENAME_BUFFER_SIZE = 256;

class Hierarchy : public EditorWindow
{
public:
    Hierarchy() = default;

    void HandleContextMenu(const char* contextMenuName);
    void CreateUndoableGameObject(const std::string& name,
                                  std::function<std::shared_ptr<GameObject>(GameObjectFactory&)> createObjectFunc, const std::string& actionDescription);
    void ProcessChildren(std::vector<std::shared_ptr<GameObject>>& objectsToRemove, ImGuiTreeNodeFlags baseFlags,
                         std::set<std::weak_ptr<Transform>, Transform::TransformCompare> children);
    void Draw() override;
    std::string GetName() override { return HIERARCHY; }
    std::weak_ptr<GameObject> GetSelectedObject();

private:
    std::weak_ptr<GameObject> selectedObject;
    std::unique_ptr<ComponentDrawerFactory> factory;
    RenamingHelper renamingHelper;
    static inline const char* DragDropPayloadID = "HIERARCHY_GAMEOBJECT";
};
