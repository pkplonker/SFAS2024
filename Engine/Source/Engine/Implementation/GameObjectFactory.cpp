#include "GameObjectFactory.h"

#include "Debug.h"
#include "IMaterial.h"
#include "MeshComponent.h"
#include "OrthographicCamera.h"
#include "PerspectiveCamera.h"
#include "ResourceManager.h"
#include "SpriteRenderable.h"
#include "Engine/IRenderable.h"
#include "Engine/Implementation/CameraComponent.h"
#include "Engine/Implementation/Scene.h"

GameObjectFactory::GameObjectFactory(std::shared_ptr<Scene> scene) : scene(scene)
{
    if (scene == nullptr || resourceManager == nullptr || graphics == nullptr)
    {
        Debug("Invalid init of GameObjectResourceManager")
        return;
    }
    gameObject = std::make_shared<GameObject>();
    SetupRandom();
}

GameObjectFactory::GameObjectFactory(std::shared_ptr<Scene> scene, std::string name) : scene(scene)
{
    if (scene == nullptr || resourceManager == nullptr || graphics == nullptr)
    {
        Debug("Invalid init of GameObjectResourceManager")
        return;
    }
    gameObject = std::make_shared<GameObject>(name);
    SetupRandom();
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
    scene->AddObject(gameObject);
    return gameObject;
}

GameObjectFactory& GameObjectFactory::AddSpriteRenderable(std::wstring shaderPath, std::wstring texturePath)
{
    auto material = resourceManager->GetMaterial(shaderPath, texturePath);
    auto component = std::make_shared<SpriteRenderable>(gameObject, graphics->CreateBillboard(material),
                                                        std::shared_ptr<IMaterial>(material));
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
    auto material = resourceManager->GetMaterial(shaderPath, texturePath);
    auto mesh = resourceManager->GetMesh(meshPath);

    auto component = std::make_shared<MeshComponent>(gameObject, graphics->CreateMeshRenderable(material, mesh),
                                                     std::shared_ptr<IMaterial>(material));
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

void GameObjectFactory::Init(IGraphics* graphics, ResourceManager* resourceManager)
{
    GameObjectFactory::graphics = graphics;
    GameObjectFactory::resourceManager = resourceManager;
}
