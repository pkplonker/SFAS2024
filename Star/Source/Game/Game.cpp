#include "Game.h"

#include "Engine/IGraphics.h"
#include "Engine/IInput.h"
#include "Engine/Implementation/GameObjectFactory.h"
#include "Engine/Implementation/Scene.h"
#include "Engine/Implementation/CameraComponent.h"
#include "Engine/Implementation/Debug.h"

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
    // scene = std::make_shared<Scene>(Graphics);
    // ResourceManager::Init(Graphics);
    // GameObjectFactory::Init(Graphics);
    //
    // camera = GameObjectFactory(scene, "Camera")
    //          .AddPerspectiveCamera()
    //          .AddPosition(Vec3(0, 0, -12.0f))
    //          .Build();
    // scene->SetActiveCamera(camera->GetComponent<CameraComponent>());
    //
    //
    // GameObjectFactory(scene, "Cube")
    //     .AddPosition(Vec3(2.5, 2.5f, 2.0f))
    //     .AddRandomRotation()
    //     .AddScale(Vec3(1))
    //     .AddMeshRenderable("S:/Users/pkplo/OneDrive/Documents/C++/SFAS2024/Editor/Resource/Mesh/TestCube.smesh",
    //                        L"S:/Users/pkplo/OneDrive/Documents/C++/SFAS2024/Editor/Resource/Shaders/UnlitColorMesh.fx")
    //     .Build();
    //
    // GameObjectFactory(scene, "Cube2")
    //     .AddPosition(Vec3(-2.5, 2.5f, 2.0f))
    //     .AddRandomRotation()
    //     .AddScale(Vec3(1))
    //     .AddMeshRenderable("S:/Users/pkplo/OneDrive/Documents/C++/SFAS2024/Editor/Resource/Mesh/TestCube.smesh",
    //                        L"S:/Users/pkplo/OneDrive/Documents/C++/SFAS2024/Editor/Resource/Shaders/UnlitColorMesh.fx")
    //     .Build();
    // GameObjectFactory(scene, "Sphere")
    //     .AddPosition(Vec3(-1, 3.5f, 2.0f))
    //     .AddRandomRotation()
    //     .AddScale(Vec3(1))
    //     .AddMeshRenderable("S:/Users/pkplo/OneDrive/Documents/C++/SFAS2024/Editor/Resource/Mesh/sphere.smesh",
    //                        L"S:/Users/pkplo/OneDrive/Documents/C++/SFAS2024/Editor/Resource/Shaders/UnlitColorMesh.fx")
    //     .Build();
    //
    // GameObjectFactory(scene, "TestShape")
    //     .AddPosition(Vec3(0, 2.5f, 2.0f))
    //     .AddRandomRotation()
    //     .AddScale(Vec3(1))
    //     .AddMeshRenderable("S:/Users/pkplo/OneDrive/Documents/C++/SFAS2024/Editor/Resource/Mesh/testshape.smesh",
    //                        L"S:/Users/pkplo/OneDrive/Documents/C++/SFAS2024/Editor/Resource/Shaders/UnlitColorMesh.fx")
    //     .Build();
    //
    // GameObjectFactory(scene, "Ground")
    //     .AddPosition(Vec3(0, -2.6f, 0.0f))
    //     .AddRotation(Vec3(45, 0, 0))
    //     .AddScale(Vec3(15.0f, 15.0f, .055f))
    //     .AddSpriteRenderable(L"S:/Users/pkplo/OneDrive/Documents/C++/SFAS2024/Editor/Resource/Shaders/UnlitColorBillboard.fx")
    //     .Build();

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

void Game::SetScene(const std::shared_ptr<Scene>& scene)
{
    Debug("Deleting scene")
    this->scene.reset();
    this->scene = scene;
    Debug("Loaded scene into game")

}
