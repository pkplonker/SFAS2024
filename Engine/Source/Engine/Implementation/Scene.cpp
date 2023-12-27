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
    objects = std::make_unique<std::map<std::string, std::shared_ptr<GameObject>>>();
}

Scene::~Scene()
{
    for (const auto object : *objects)
    {
        auto renderable = object.second->GetComponent<IRenderableComponent>();
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
    objects->emplace(object->GetGUID(), object);
}

void Scene::RemoveObject(std::shared_ptr<GameObject> object)
{
    if (object == nullptr)return;
    auto renderable = object->GetComponent<IRenderableComponent>();

    if (objects->contains(object->GetGUID()))
    {
        if (renderable != nullptr)
        {
            RemoveRenderable(renderable->GetRenderable());
        }
        objects->erase(object->GetGUID());
    }
    else
    {
        Warning("GameObject not found to delete");
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
    for (const auto& object : *objects)
    {
        object.second->Update();
    }
}

void Scene::SetActiveCamera(const std::shared_ptr<ICamera>& camera)
{
    this->camera = camera;
    camera->SetWidth(static_cast<float>(IApplication::GetGraphics()->GetWidth()));
    camera->SetHeight(static_cast<float>(IApplication::GetGraphics()->GetHeight()));
    IApplication::GetGraphics()->SetActiveCamera(camera);
}

std::map<std::string, std::shared_ptr<GameObject>>& Scene::GetObjects() const
{
    return *objects;
}

std::shared_ptr<ICamera> Scene::GetActiveCamera()
{
    return camera;
}

bool Scene::TryFindObject(const std::string& string, std::weak_ptr<GameObject>& object) const
{
    if(objects->contains(string))
    {
        object= (*objects)[string];
        return true;
    }
    return false;
}
