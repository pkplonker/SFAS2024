#pragma once
#include <memory>
#include <set>

#include "Engine/GameObject.h"
#include "Engine/IImGuiRenderable.h"
#include "Engine/IRenderable.h"

class ImGuiController;

class Scene : public IUpdateable, public IImGuiRenderable
{
public:
    Scene(IGraphics* graphics, ImGuiController* ImGui);
    void AddObject(std::shared_ptr<GameObject> object);
    void AddRenderable(std::shared_ptr<IRenderable> object) const;
    void Update() override;
    void AddRenderable(const std::shared_ptr<GameObject>& object) const;
    void SetActiveCamera(const std::shared_ptr<ICamera>& camera);
    void DrawScene();
    void DrawInspector();
    void DrawCamera();
    void ImGuiRender(std::string window) override;
private:
    std::unique_ptr<std::set<std::shared_ptr<GameObject>>> objects = std::make_unique<std::set<std::shared_ptr<
        GameObject>>>();
    std::unique_ptr<std::set<std::shared_ptr<IRenderable>>> renderables = std::make_unique<std::set<std::shared_ptr<
        IRenderable>>>();
    IGraphics* graphics;
    std::shared_ptr<ICamera> camera;
    ImGuiController* ImGui;
    std::weak_ptr<GameObject> selectedObject; 

};
