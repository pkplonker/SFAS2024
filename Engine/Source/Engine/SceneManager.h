#pragma once
#include <memory>

#include "Event.h"

class Scene;

class SceneManager
{
public:
    static std::weak_ptr<Scene> GetScene();
    static void SetScene(Scene* scene);
    static void SetScene(std::shared_ptr<Scene> scene);
    static inline Event OnSceneChangedEvent;

private:
    inline static std::shared_ptr<Scene> activeScene = nullptr;
};
