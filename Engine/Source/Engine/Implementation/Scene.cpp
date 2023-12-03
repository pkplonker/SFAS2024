#include "Scene.h"

#include "CameraComponent.h"
#include "MeshComponent.h"
#include "SpriteComponent.h"
#include "Engine/Implementation/Logging/Debug.h"
#include "Engine/ICamera.h"
#include "Engine/IApplication.h"
class SpriteComponent;

Scene::Scene()
{
    objects = std::make_unique<std::set<std::shared_ptr<
        GameObject>>>();
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
}

void Scene::RemoveObject(std::shared_ptr<GameObject> object)
{
    auto it = std::find(objects->begin(), objects->end(), object);
    auto renderable = object->GetComponent<IRenderableComponent>();
    if (it != objects->end())
    {
        if (renderable != nullptr)
        {
            RemoveRenderable(renderable->GetRenderable());
        }
        objects->erase(it);
    }
    else
    {
        Warning("Renderable not found to delete");
    }
}

void Scene::RemoveRenderable(std::shared_ptr<IRenderable> object) const
{
    auto graphics = IApplication::GetGraphics();
    if (graphics != nullptr)
    {
        IApplication::GetGraphics()->RemoveRenderable(object);
    }
    else
    {
        Warning("Renderable not found to delete");
    }
}

void Scene::Update()
{
    for (const std::shared_ptr<GameObject>& object : *objects)
    {
        object->Update();
    }
}

void Scene::SetActiveCamera(const std::shared_ptr<ICamera>& camera)
{
    this->camera = camera;
    camera->SetWidth(IApplication::GetGraphics()->GetWidth());
    camera->SetHeight(IApplication::GetGraphics()->GetHeight());
    IApplication::GetGraphics()->SetActiveCamera(camera);
}

std::set<std::shared_ptr<GameObject>>& Scene::GetObjects() const
{
    return *objects;
}

std::shared_ptr<ICamera> Scene::GetActiveCamera()
{
    return camera;
}
