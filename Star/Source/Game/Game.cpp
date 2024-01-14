#include "Game.h"

#include "SceneSerializer.h"
#include "Engine/IGraphics.h"
#include "Engine/IInput.h"
#include "Engine/SceneManager.h"
#include "Engine/Implementation/GameObjectFactory.h"
#include "Engine/Implementation/Helpers.h"
#include "Engine/Implementation/Scene.h"
#include "Engine/Implementation/Logging/Debug.h"

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
    SceneManager::SetScene(SceneSerializer::Deserialize(Helpers::ToAbsolutePath("Resource/Scenes/SceneComp.scene")));

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

void Game::Resize(int width, int height)
{
    Graphics->Resize(width,height);
}


