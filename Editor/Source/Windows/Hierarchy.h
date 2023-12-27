#pragma once
#include <memory>

#include "ComponentDrawerFactory.h"
#include "EditorWindow.h"
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
    void Draw() override;
    std::string GetName() override { return HIERARCHY; }
    std::weak_ptr<GameObject> GetSelectedObject();

private:
    std::weak_ptr<GameObject> selectedObject;
    std::unique_ptr<ComponentDrawerFactory> factory;
    RenamingHelper renamingHelper;

};
