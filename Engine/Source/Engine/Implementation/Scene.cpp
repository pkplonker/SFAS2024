#include "Scene.h"

#include "CameraComponent.h"
#include "MeshComponent.h"
#include "SpriteComponent.h"
#include "Engine/Implementation/Debug.h"
#include "Engine/ICamera.h"

class SpriteComponent;

Scene::Scene(IGraphics* graphics)
{
    this->graphics = graphics;
    objects = std::make_unique<std::set<std::shared_ptr<
        GameObject>>>();
    renderables = std::make_unique<std::set<std::shared_ptr<
        IRenderable>>>();
}

Scene::~Scene()
{
    for (const auto object : *objects)
    {
        auto renderable = object->GetComponent<IRenderableComponent>();
        if (renderable != nullptr)
        {
            RemoveRenderable(renderable->GetRenderable());
        }
    }
}

void Scene::AddObject(std::shared_ptr<GameObject> object)
{
    if (this->camera == nullptr)
    {
        auto cameraComponent = object->GetComponent<CameraComponent>();
        if (cameraComponent != nullptr)
        {
            this->SetActiveCamera(cameraComponent);
        }
    }
    objects->emplace(object);
    AddRenderable(object);
}

void Scene::RemoveObject(std::shared_ptr<GameObject> object)
{
    auto it = std::find(objects->begin(), objects->end(), object);
    auto renderable = object->GetComponent<IRenderableComponent>();
    if (it != objects->end())
    {
        objects->erase(it);
        if (renderable != nullptr)
        {
            RemoveRenderable(renderable->GetRenderable());
        }
    }
    else
    {
        Debug("Renderable not found to delete");
    }
}

void Scene::AddRenderable(std::shared_ptr<IRenderable> object) const
{
    renderables->emplace(object);
}

void Scene::RemoveRenderable(std::shared_ptr<IRenderable> object) const
{
    auto it = std::find(renderables->begin(), renderables->end(), object);
    if (it != renderables->end())
    {
        renderables->erase(it);
        graphics->RemoveRenderable(object);
    }
    else
    {
        Debug("Renderable not found to delete");
    }
}

void Scene::Update()
{
    for (const std::shared_ptr<GameObject>& object : *objects)
    {
        object->Update();
    }
}

void Scene::AddRenderable(const std::shared_ptr<GameObject>& object) const
{
    std::shared_ptr<IRenderableComponent> renderableComponent = object->GetComponent<IRenderableComponent>();
    if (renderableComponent != nullptr)
    {
        AddRenderable(renderableComponent->GetRenderable());
    }
}

void Scene::SetActiveCamera(const std::shared_ptr<ICamera>& camera)
{
    this->camera = camera;
    camera->SetWidth(graphics->GetWidth());
    camera->SetHeight(graphics->GetHeight());
    graphics->SetActiveCamera(camera);
}

std::set<std::shared_ptr<GameObject>>& Scene::GetObjects() const
{
    return *objects;
}

std::shared_ptr<ICamera> Scene::GetActiveCamera()
{
    return camera;
}
