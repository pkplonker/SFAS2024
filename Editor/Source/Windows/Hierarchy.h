#pragma once
#include <memory>

#include "ComponentDrawerFactory.h"
#include "ImGuiWindow.h"
class GameObject;
class Scene;
const std::string HIERARCHY = "Hierarchy";

class Hierarchy : public EditorWindow
{
public:
    Hierarchy(const std::weak_ptr<Scene>& scene);

    void Draw() override;
    std::string GetName() override { return HIERARCHY; }
    std::weak_ptr<GameObject> GetSelectedObject();
    void SetScene(std::weak_ptr<Scene> value);

private:
    std::weak_ptr<Scene> scene;
    std::weak_ptr<GameObject> selectedObject;
    std::unique_ptr<ComponentDrawerFactory> factory;
};
