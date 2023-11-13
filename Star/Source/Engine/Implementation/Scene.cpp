#include "Scene.h"

#include "MeshRenderable.h"
#include "SpriteRenderable.h"
#include "Engine/ICamera.h"

class SpriteRenderable;

Scene::Scene(IGraphics* graphics)
{
    objects = std::make_unique<std::set<std::shared_ptr<
        GameObject>>>();
    renderables = std::make_unique<std::set<std::shared_ptr<
        IRenderable>>>();
    this->graphics = graphics;
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

void Scene::SetActiveCamera(std::shared_ptr<ICamera> camera)
{
    this->camera=camera;
    camera->SetWidth(graphics->GetWidth());
    camera->SetHeight(graphics->GetHeight());
    graphics->SetActiveCamera(camera);
}

