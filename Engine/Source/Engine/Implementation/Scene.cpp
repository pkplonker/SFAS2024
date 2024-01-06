#include "Scene.h"

#include "CameraComponent.h"
#include "DirectionalLightComponent.h"
#include "MeshComponent.h"
#include "SpriteComponent.h"
#include "Engine/Implementation/Logging/Debug.h"
#include "Engine/ICamera.h"
#include "Engine/IApplication.h"
class SpriteComponent;

Scene::Scene(IGraphics* graphics, std::string path) : filePath(path), ambientLightColor(Vec3(1.0f, 1.0f, 1.0f)),
                                                      ambientLightIntensity(1)
{
    objects = std::make_unique<std::map<std::string, std::shared_ptr<GameObject>>>();
    this->graphics = graphics;
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
    if (object == nullptr)return;
    if (this->camera == nullptr)
    {
        auto cameraComponent = object->GetComponent<CameraComponent>();
        if (cameraComponent != nullptr)
        {
            this->SetActiveCamera(cameraComponent);
        }
    }
    objects->emplace(object->GetGUID(), object);

    if (graphics != nullptr)
    {
        const auto renderable = object->GetComponent<IRenderableComponent>();
        if (renderable != nullptr)
        {
            graphics->UpdateRenderable(renderable->GetMaterial(), renderable->GetRenderable());
        }
    }
}

void Scene::RegisterDirectionalLight(std::weak_ptr<DirectionalLightComponent> dirLightComponent)
{
    if (const auto lightComponent = dirLightComponent.lock())
    {
        if (directionalLight.lock() == nullptr)
        {
            this->directionalLight = lightComponent;
        }
        else
        {
            Warning("Secound directional light added, only the first light will be used.")
        }
    }
}

std::weak_ptr<DirectionalLightComponent> Scene::GetDirectionalLight()
{
    return directionalLight;
}

Vec3 Scene::GetAmbientLightColor()
{
    return ambientLightColor;
}

float Scene::GetAmbientLightIntensity()
{
    return ambientLightIntensity;
}

void Scene::SetAmbientLightColor(Vec3 value)
{
    ambientLightColor = value;
}

void Scene::SetAmbientLightIntensity(float value)
{
    ambientLightIntensity = value;
}

void Scene::RegisterLight(const std::shared_ptr<ILight>& light)
{
    auto it = std::find(lights.begin(), lights.end(), light);

    if (it == lights.end())
    {
        lights.push_back(light);
    }
}

void Scene::DeregisterLight(const std::shared_ptr<ILight>& light)
{
    std::erase(lights, light);
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
        object->Transform()->SetParent(nullptr);
        objects->erase(object->GetGUID());
    }
    else
    {
        Warning("GameObject not found to delete");
    }
}

void Scene::RemoveObject(std::string guid)
{
    if (guid == "")return;
    if (objects->contains(guid))
    {
        auto object = (*objects)[guid];
        auto renderable = object->GetComponent<IRenderableComponent>();
        if (renderable != nullptr)
        {
            RemoveRenderable(renderable->GetRenderable());
        }
        object->Transform()->SetParent(nullptr);
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
    if (objects->contains(string))
    {
        object = (*objects)[string];
        return true;
    }
    return false;
}

std::shared_ptr<GameObject> Scene::GetGameObject(const std::string& string) const
{
    if (objects->contains(string))
    {
        return (*objects)[string];
    }
    return nullptr;
}
