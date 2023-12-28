#pragma once
#include <memory>
#include <set>

#include "Engine/Implementation/GameObject.h"
#include "Engine/IRenderable.h"

class ImGuiController;

class Scene : public IUpdateable, public Transform
{
public:
    Scene(IGraphics* graphics);
    ~Scene() override;

    void SetScene()
    {
    }

    void AddObject(std::shared_ptr<GameObject> object);
    void RemoveObject(std::shared_ptr<GameObject> object);
    void RemoveObject(std::string guid);
    void RemoveRenderable(std::shared_ptr<IRenderable> object) const;
    void Update() override;
    void SetActiveCamera(const std::shared_ptr<ICamera>& camera);
    std::map<std::string,std::shared_ptr<GameObject>>& GetObjects() const;
    std::shared_ptr<ICamera> GetActiveCamera();
    bool TryFindObject(const std::string& string,std::weak_ptr<GameObject>& object) const;

private:
    std::unique_ptr<std::map<std::string,std::shared_ptr<GameObject>>> objects = std::make_unique<std::map<std::string,std::shared_ptr<
        GameObject>>>();
    std::shared_ptr<ICamera> camera = {};
    std::weak_ptr<GameObject> selectedObject = {};
    IGraphics* graphics;
};
