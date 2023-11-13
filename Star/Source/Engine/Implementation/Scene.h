#pragma once
#include <memory>
#include <set>

#include "Engine/GameObject.h"
#include "Engine/IRenderable.h"

class Scene : public IUpdateable
{
public:
    Scene(IGraphics* graphics);
    void AddObject(std::shared_ptr<GameObject> object);
    void AddRenderable(std::shared_ptr<IRenderable> object) const;
    void Update() override;
    void AddRenderable(std::shared_ptr<GameObject> object) const;
    void SetActiveCamera(std::shared_ptr<ICamera> camera);
private:
    std::unique_ptr<std::set<std::shared_ptr<GameObject>>> objects = std::make_unique<std::set<std::shared_ptr<
        GameObject>>>();
    std::unique_ptr<std::set<std::shared_ptr<IRenderable>>> renderables = std::make_unique<std::set<std::shared_ptr<
        IRenderable>>>();
    IGraphics* graphics;
    std::shared_ptr<ICamera> camera;
};
