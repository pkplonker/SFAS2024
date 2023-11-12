#include "Scene.h"

#include "MeshRenderable.h"
#include "SpriteRenderable.h"

class SpriteRenderable;

Scene::Scene()
{
    objects = std::make_unique<std::set<std::shared_ptr<
        GameObject>>>();
    renderables = std::make_unique<std::set<std::shared_ptr<
        IRenderable>>>();
}

void Scene::AddObject(std::shared_ptr<GameObject> object)
{
    objects->emplace(object);
    AddRenderable(object);
}

void Scene::AddRenderable(std::shared_ptr<IRenderable> object) const
{
    renderables->emplace(object);
}

void Scene::Update()
{
    for (const std::shared_ptr<GameObject>& object : *objects)
    {
        object->Update();
    }
}

void Scene::AddRenderable(std::shared_ptr<GameObject> object) const
{
    std::shared_ptr<IRenderableComponent> renderableComponent = object->GetComponent<IRenderableComponent>();
    if (renderableComponent != nullptr)
    {
        renderables->emplace(renderableComponent->GetRenderable());
    }
}
