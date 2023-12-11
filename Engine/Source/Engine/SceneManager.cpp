#include "SceneManager.h"

#include "Implementation/Scene.h"

std::weak_ptr<Scene> SceneManager::GetScene()
{
    return activeScene;
}

void SceneManager::SetScene(Scene* scene)
{
    SceneManager::activeScene = std::shared_ptr<Scene>(scene);
    OnSceneChangedEvent.Invoke();
}

void SceneManager::SetScene(std::shared_ptr<Scene> scene)
{
    SceneManager::activeScene = scene;
    OnSceneChangedEvent.Invoke();
}
