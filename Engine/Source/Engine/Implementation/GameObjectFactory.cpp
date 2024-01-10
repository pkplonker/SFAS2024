#include "GameObjectFactory.h"

#include "DirectionalLightComponent.h"
#include "IApplication.h"
#include "Engine/Implementation/Logging/Debug.h"
#include "IMaterial.h"
#include "MeshComponent.h"
#include "OrthographicCamera.h"
#include "PerspectiveCamera.h"
#include "PointLightComponent.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "SpotLightComponent.h"
#include "SpriteComponent.h"
#include "Engine/IRenderable.h"
#include "Engine/Implementation/CameraComponent.h"
#include "Engine/Implementation/Scene.h"

void GameObjectFactory::Setup(std::string name)
{
    gameObject = std::make_shared<GameObject>(name);
    gameObject->Init();
    auto scene = SceneManager::GetScene().lock();
    if (scene == nullptr)
    {
        Warning("no scene to create gameobject");
        return;
    }
    gameObject->Transform()->SetParent(scene);
}

GameObjectFactory::GameObjectFactory()
{
    Setup(GenerateName());
}

std::string GameObjectFactory::GenerateName() const
{
    auto scene = SceneManager::GetScene().lock();
    if (scene == nullptr)
    {
        Warning("no scene to create gameobject");
        return "";
    }
    int counter = 1;
    std::string baseName = GameObject::GAMEOBJECT_DEFAULT_NAME;
    auto name = baseName + " (" + std::to_string(counter) + ")";

    while (ObjectNameExists(scene, name))
    {
        counter++;
        name = baseName + " (" + std::to_string(counter) + ")";
    }
    return name;
}


GameObjectFactory::GameObjectFactory(std::string name)
{
    auto scene = SceneManager::GetScene().lock();
    if (scene == nullptr)
    {
        Warning("no scene to create gameobject");
        return;
    }
    gameObject = std::make_shared<GameObject>(name);
    gameObject->Init();
    gameObject->Transform()->SetParent(scene);

    SetupRandom();
}

bool GameObjectFactory::ObjectNameExists(const std::shared_ptr<Scene>& scene, const std::string& name)
{
    for (const auto& [guid, object] : scene->GetObjects())
    {
        if (object->Name == name)
        {
            return true;
        }
    }
    return false;
}

void GameObjectFactory::SetupRandom()
{
    if (random == false)
    {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        random = true;
    }
}

GameObjectFactory& GameObjectFactory::AddPosition(Vec3 vec)
{
    gameObject->Transform()->Position = vec;
    return *this;
}

GameObjectFactory& GameObjectFactory::AddRotation(Vec3 vec)
{
    gameObject->Transform()->Rotation = vec;
    return *this;
}

GameObjectFactory& GameObjectFactory::SetParentWeak(std::weak_ptr<Transform> transform)
{
    if (const auto sharedTrans = transform.lock())
    {
        return SetParent(sharedTrans);
    }
    return *this;
}

GameObjectFactory& GameObjectFactory::SetParent(std::shared_ptr<Transform> transform)
{
    gameObject->Transform()->SetParent(transform);
    return *this;
}


GameObjectFactory& GameObjectFactory::AddScale(Vec3 vec)
{
    gameObject->Transform()->Scale = vec;
    return *this;
}

GameObjectFactory& GameObjectFactory::AddName(std::string name)
{
    gameObject->Name = name;
    return *this;
}

std::shared_ptr<GameObject> GameObjectFactory::Build()
{
    if (auto scene = SceneManager::GetScene().lock())
    {
        scene->AddObject(gameObject);
        return gameObject;
    }
    return nullptr;
}

GameObjectFactory& GameObjectFactory::AddSpriteRenderable(std::wstring shaderPath, std::wstring texturePath)
{
    auto material = ResourceManager::GetMaterial(shaderPath, texturePath);
    auto component = std::make_shared<SpriteComponent>(
        gameObject, IApplication::GetGraphics()->CreateBillboard(material),
        material);
    if (component != nullptr)
    {
        gameObject->AddComponent(std::move(component));
        return *this;
    }
    return *this;
}

GameObjectFactory& GameObjectFactory::AddSpriteRenderable(std::wstring shaderPath)
{
    return AddSpriteRenderable(shaderPath, L"");
}

GameObjectFactory& GameObjectFactory::AddMeshRenderable(std::string meshPath, std::wstring shaderPath,
                                                        std::wstring texturePath)
{
    auto material = ResourceManager::GetMaterial(shaderPath, texturePath);
    auto mesh = ResourceManager::GetMesh(meshPath);

    auto component = std::make_shared<MeshComponent>(
        gameObject, IApplication::GetGraphics()->CreateMeshRenderable(material, mesh),
        material);
    if (component != nullptr)
    {
        gameObject->AddComponent(std::move(component));
        return *this;
    }
    return *this;
}

GameObjectFactory& GameObjectFactory::AddMeshRenderable(Mesh* mesh, std::wstring shaderPath = L"")
{
    IMaterial* material;
    if (shaderPath == L"")
    {
        material = ResourceManager::GetMaterial();
    }
    else
    {
        material = ResourceManager::GetMaterial(shaderPath);
    }

    auto component = std::make_shared<MeshComponent>(
        gameObject, IApplication::GetGraphics()->CreateMeshRenderable(material, mesh),
        material);
    if (component != nullptr)
    {
        gameObject->AddComponent(std::move(component));
        return *this;
    }
    return *this;
}

GameObjectFactory& GameObjectFactory::AddMeshRenderable(Mesh* mesh)
{
    IMaterial* material;

    material = ResourceManager::GetMaterial();

    auto component = std::make_shared<MeshComponent>(
        gameObject, IApplication::GetGraphics()->CreateMeshRenderable(material, mesh),
        material);
    if (component != nullptr)
    {
        gameObject->AddComponent(std::move(component));
        return *this;
    }
    return *this;
}

GameObjectFactory& GameObjectFactory::AddEmptyMeshRenderable()
{
    auto component = std::make_shared<MeshComponent>(
        gameObject, nullptr,
        nullptr);
    if (component != nullptr)
    {
        gameObject->AddComponent(std::move(component));
        return *this;
    }
    return *this;
}

GameObjectFactory& GameObjectFactory::AddMeshRenderable(std::string meshPath, std::wstring shaderPath)
{
    return AddMeshRenderable(meshPath, shaderPath, L"");
}

GameObjectFactory& GameObjectFactory::AddRandomRotation()
{
    gameObject->Transform()->Rotation = Vec3(static_cast<float>(std::rand() % 361),
                                             static_cast<float>(std::rand() % 361),
                                             static_cast<float>(std::rand() % 361));
    return *this;
}

GameObjectFactory& GameObjectFactory::AddPerspectiveCamera()
{
    auto perspective = std::make_shared<PerspectiveCamera>(gameObject->Transform(), 0, 0);
    auto component = std::make_shared<CameraComponent>(gameObject, perspective);
    if (component != nullptr)
    {
        gameObject->AddComponent(std::move(component));
        return *this;
    }
    return *this;
}

GameObjectFactory& GameObjectFactory::AddOrthoCamera()
{
    auto ortho = std::make_shared<OrthographicCamera>(gameObject->Transform(), 0, 0);

    auto component = std::make_shared<CameraComponent>(gameObject, ortho);
    if (component != nullptr)
    {
        gameObject->AddComponent(std::move(component));
        return *this;
    }
    return *this;
}

GameObjectFactory& GameObjectFactory::AddEmptySpriteRenderable()
{
    auto component = std::make_shared<SpriteComponent>(
        gameObject, nullptr,
        nullptr);
    if (component != nullptr)
    {
        gameObject->AddComponent(std::move(component));
        return *this;
    }
    return *this;
}

GameObjectFactory& GameObjectFactory::AddDirectionalLight()
{
    auto component = std::make_shared<DirectionalLightComponent>(
        gameObject);
    if (component != nullptr)
    {
        gameObject->AddComponent(std::move(component));
        return *this;
    }
    return *this;
}

GameObjectFactory& GameObjectFactory::AddSpotLight()
{
    auto component = std::make_shared<SpotLightComponent>(
        gameObject);
    if (component != nullptr)
    {
        gameObject->AddComponent(std::move(component));
        return *this;
    }
    return *this;
}

GameObjectFactory& GameObjectFactory::AddPointLight()
{
    auto component = std::make_shared<PointLightComponent>(
        gameObject);
    if (component != nullptr)
    {
        gameObject->AddComponent(std::move(component));
        return *this;
    }
    return *this;
}
