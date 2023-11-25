#include "Game.h"

#include "Engine/IGraphics.h"
#include "Engine/IInput.h"
#include "Engine/MeshSerializer.h"
#include "Engine/ResourceManager.h"
#include "Engine/Implementation/GameObjectFactory.h"
#include "Engine/Implementation/Scene.h"
#include "Engine/Implementation/CameraComponent.h"

#define CLAMP(v, x, y) fmin(fmax(v, x), y)

constexpr float PieVal = 3.14159265359f;
constexpr float TwoPies = PieVal * 2.0f;
constexpr float DeltaTime = 0.016f;
constexpr float SpinSpeed = 0.1f;
constexpr float WinTolerance = PieVal / 10.0f;

IApplication* GetApplication(IGraphics* Graphics, IInput* Input)
{
    return new Game(Graphics, Input);
}

Game::Game(IGraphics* GraphicsIn, IInput* InputIn) : IApplication(GraphicsIn, InputIn)
{
}

Game::~Game()
{
}

bool Game::IsValid()
{
    return true;
}

bool Game::Load()
{
    scene = std::make_shared<Scene>(Graphics);
    resourceManager = std::make_unique<ResourceManager>(Graphics);;

    camera = GameObjectFactory(scene, "Camera")
             .AddPerspectiveCamera()
             .AddPosition(Vec3(0, 0, -12.0f))
             .Build();
    scene->SetActiveCamera(camera->GetComponent<CameraComponent>());


    IMaterial* material = resourceManager->GetMaterial(L"Resource/Shaders/UnlitColor5.fx");
    GameObjectFactory(scene, "Red")
        .AddPosition(Vec3(0, 1.5f, 2.0f))
        .AddRandomRotation()
        .AddScale(Vec3(1, 1, 1))
        .AddMeshRenderable(std::shared_ptr<IMaterial>(material), Graphics->CreateMeshRenderable(material))
        .Build();


    material = resourceManager->GetMaterial(L"Resource/Shaders/UnlitColorMesh.fx");

    Mesh* mesh = MeshSerializer::Deserialize(
        "S:/Users/pkplo/OneDrive/Documents/C++/SFAS2024/Editor/Resource/Mesh/TestCube.smesh");

    GameObjectFactory(scene, "TestCube")
        .AddPosition(Vec3(3.5, 1.5f, 2.0f))
        .AddRandomRotation()
        .AddScale(Vec3(1))
        .AddMeshRenderable(std::shared_ptr<IMaterial>(material), Graphics->CreateMeshRenderable(material, mesh))
        .Build();

    auto mesh2 = MeshSerializer::Deserialize(
        "S:/Users/pkplo/OneDrive/Documents/C++/SFAS2024/Editor/Resource/Mesh/testshape.smesh");
    GameObjectFactory(scene, "TestShape")
        .AddPosition(Vec3(2.5, 2.5f, 2.0f))
        .AddRandomRotation()
        .AddScale(Vec3(1))
        .AddMeshRenderable(std::shared_ptr<IMaterial>(material), Graphics->CreateMeshRenderable(material, mesh2))
        .Build();

    material = resourceManager->GetMaterial(L"Resource/Shaders/UnlitColor3.fx");
    GameObjectFactory(scene, "Ground")
        .AddPosition(Vec3(0, -2.6f, 0.0f))
        .AddRotation(Vec3(45, 0, 0))
        .AddScale(Vec3(15.0f, 15.0f, .055f))
        .AddSpriteRenderable(std::shared_ptr<IMaterial>(material), Graphics->CreateBillboard(material))
        .Build();

    return true;
}

void Game::Update()
{
}

void Game::Cleanup()
{
}

void Game::PostGraphics()
{
}

std::weak_ptr<Scene> Game::GetScene()
{
    return scene;
}
