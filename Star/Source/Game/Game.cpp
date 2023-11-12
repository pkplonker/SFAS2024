#include "Game.h"

#include "Engine/IGraphics.h"
#include "Engine/IRenderable.h"
#include "Engine/IInput.h"

#include <ctime>

#include "Engine/Debug.h"
#include "Engine/GameObject.h"
#include "Engine/GameObjectFactory.h"
#include "Engine/ResourceManager.h"

#define CLAMP(v, x, y) fmin(fmax(v, x), y)

constexpr float Pie = 3.14159265359f;
constexpr float TwoPies = Pie * 2.0f;
constexpr float DeltaTime = 0.016f;
constexpr float SpinSpeed = 0.1f;
constexpr float WinTolerance = Pie / 10.0f;

IApplication* GetApplication(IGraphics* Graphics, IInput* Input)
{
    return new Game(Graphics, Input);
}

Game::Game(IGraphics* GraphicsIn, IInput* InputIn) : IApplication(GraphicsIn, InputIn), Rings(), Arrow(nullptr),
                                                     SelectedRing(), State()
{
    scene = std::make_shared<Scene>();
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
    scene = std::make_shared<Scene>();
    resourceManager = std::make_unique<ResourceManager>(Graphics);;


    // Rings[static_cast<unsigned int>(Inner)] = GameObjectFactory(scene, "Inner")
    //                                           .AddPosition(Vec3(0, 0, 1.0f))
    //                                           .AddSpriteRenderable(Graphics->CreateBillboard(
    //                                               resourceManager->GetShader(
    //                                                   L"Resource/Textures/InnerRing.dds",
    //                                                   L"Resource/Shaders/UnlitColor.fx")))
    //                                           .Build();
    // Rings[static_cast<unsigned int>(Middle)] = GameObjectFactory(scene, "Middle")
    //                                            .AddPosition(Vec3(0, 0, 1.0f))
    //                                            .AddSpriteRenderable(Graphics->CreateBillboard(
    //                                                resourceManager->GetShader(
    //                                                    L"Resource/Textures/MiddleRing.dds",
    //                                                    L"Resource/Shaders/UnlitColor.fx")))
    //                                            .Build();
    // Rings[static_cast<unsigned int>(Outer)] = GameObjectFactory(scene, "Outer")
    //                                           .AddPosition(Vec3(0, 0, 1.0f))
    //                                           .AddSpriteRenderable(Graphics->CreateBillboard(
    //                                               resourceManager->GetShader(
    //                                                   L"Resource/Textures/OuterRing.dds",
    //                                                   L"Resource/Shaders/UnlitColor.fx")))
    //                                           .Build();
    //
    // Arrow = GameObjectFactory(scene, "Arrow")
    //         .AddPosition(Vec3(0, 0, 1.0f))
    //         .AddSpriteRenderable(Graphics->CreateBillboard(
    //             resourceManager->GetShader(L"Resource/Textures/Arrow.dds", L"Resource/Shaders/UnlitColor.fx")))
    //         .Build();

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    SelectedRing = RingLayer::Outer;
    State = GameState::Setup;
    testObjects.emplace_back(GameObjectFactory(scene, std::to_string(testObjects.size()))
                             .AddPosition(Vec3(0, 0, 10.0f))
                             .AddRotation(Vec3(45.0f, 45.0f, 45.0f))
                             .AddScale(Vec3(1,1,1))
                             .AddMeshRenderable(Graphics->CreateMeshRenderable(resourceManager->GetShader(
                                 L"Resource/Textures/Cat.dds",
                                 L"Resource/Shaders/UnlitColor2.fx")))
                             .Build());

    // testObjects.emplace_back(GameObjectFactory(scene, std::to_string(testObjects.size()))
    //                          .AddPosition(Vec3(450, 150, 10.0f))
    //                          .AddRotation(Vec3(45.0f, 45.0f, 45.0f))
    //                          .AddScale(Vec3(10,10,10))
    //                          .AddMeshRenderable(Graphics->CreateMeshRenderable(resourceManager->GetShader(
    //                              L"Resource/Textures/Cat.dds",
    //                              L"Resource/Shaders/UnlitColor2.fx")))
    //                          .Build());
    // testObjects.emplace_back(GameObjectFactory(scene, std::to_string(testObjects.size()))
    //                   .AddPosition(Vec3(450, 150, 10.0f))
    //                   .AddRotation(Vec3(45.0f,45.0f,45.0f))
    //                   .AddScale(Vec3(5))
    //
    //                   .AddMeshRenderable(Graphics->CreateMeshRenderable(resourceManager->GetShader(
    //                       L"Resource/Textures/Cat.dds",
    //                       L"Resource/Shaders/UnlitColor2.fx")))
    //                   .Build());
    // testObjects.emplace_back(GameObjectFactory(scene, std::to_string(testObjects.size()))
    //                   .AddPosition(Vec3(-450, 150, 3.0f))
    //                   .AddRotation(Vec3(45.0f,45.0f,45.0f))
    //                   .AddScale(Vec3(20))
    //                   .AddMeshRenderable(Graphics->CreateMeshRenderable(resourceManager->GetShader(
    //                       L"Resource/Textures/Cat.dds",
    //                       L"Resource/Shaders/UnlitColor2.fx")))
    //                   .Build());
    // testObjects.emplace_back(GameObjectFactory(scene, std::to_string(testObjects.size()))
    //               .AddPosition(Vec3(-450, 150, -20.0f))
    //               .AddRotation(Vec3(45.0f,45.0f,45.0f))
    //               .AddScale(Vec3(10))
    //               .AddMeshRenderable(Graphics->CreateMeshRenderable(resourceManager->GetShader(
    //                   L"Resource/Textures/Cat.dds",
    //                   L"Resource/Shaders/UnlitColor2.fx")))
    //               .Build());
    return true;
}

void Game::Update()
{
    // // If mode is Setup game then set each ring to a random rotation
    // if (State == GameState::Setup)
    // {
    //     SetupEachRing();
    //     State = GameState::Playing;
    // }
    //
    // // If mode is Playing then read controller input and manage which ring is selected, the rotation of each ring and waiting for select to confirm positions
    // if (State == GameState::Playing)
    // {
    //     UpdateRingSelection();
    //     UpdateSelectedRingRotation();
    //     UpdateRingTestSelection();
    // }
    //
    // // If mode is Test then check to see if the rings are in their correct positions, play a noise corresponding to how close the player is
    // if (State == GameState::Test)
    // {
    //     TestRingSolution();
    //     State = GameState::Setup;
    // }
}

void Game::Cleanup()
{
}

void Game::SetupEachRing()
{
    for (unsigned int Ring = 0; Ring < NumberOfRings; ++Ring)
    {
        Rings[Ring]->Transform()->Rotation = Vec3(0, 0, static_cast<float>(fmod(rand(), Pie)));
    }

    Arrow->Transform()->Rotation = Vec3(0, 0, static_cast<float>(fmod(rand(), Pie)));
}

void Game::UpdateRingSelection()
{
    int selectionChange = 0;

    if (Input->IsPressed(InputAction::ShoulderButtonLeft))
    {
        // Change ring selection towards outer
        selectionChange = -1;
    }
    else if (Input->IsPressed(InputAction::ShoulderButtonRight))
    {
        // Change ring selection towards inner
        selectionChange = 1;
    }

    SelectedRing = static_cast<RingLayer>(
        CLAMP(static_cast<int>(SelectedRing) + selectionChange, 0, NumberOfRings - 1));
}

void Game::UpdateSelectedRingRotation()
{
    float delta = Input->GetValue(InputAction::RightStickXAxis) * SpinSpeed * DeltaTime;
    Vec3 rotation = Rings[static_cast<int>(SelectedRing)]->Transform()->Rotation;
    Vec3 newRotation = Vec3(0, 0, static_cast<float>(fmod(rotation.Z() + delta, TwoPies)));
    Rings[static_cast<int>(SelectedRing)]->Transform()->Rotation = newRotation;
}

void Game::UpdateRingTestSelection()
{
    if (Input->IsPressed(InputAction::DefaultSelect))
    {
        State = GameState::Test;
    }
}

void Game::TestRingSolution()
{
    float totalRotationDifference = 0.0f;
    float arrowRotation = Arrow->Transform()->Rotation.Z() + TwoPies;

    for (unsigned int Ring = 0; Ring < NumberOfRings; ++Ring)
    {
        totalRotationDifference += abs(arrowRotation - (Rings[Ring]->Transform()->Rotation.Z() + TwoPies));
    }

    float averageRotationDifference = totalRotationDifference / NumberOfRings;

    if (averageRotationDifference < WinTolerance)
    {
        Debug("win")
    }
    else
    {
        Debug("Lose")
    }
}
