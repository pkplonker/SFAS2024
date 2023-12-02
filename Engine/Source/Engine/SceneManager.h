#pragma once
#include <memory>

class Scene;

class SceneManager
{
public:
    static std::weak_ptr<Scene> GetScene();
    static void SetScene(Scene* scene);
    static void SetScene(std::shared_ptr<Scene> scene);

private:
    inline static std::shared_ptr<Scene> activeScene = nullptr;
};
