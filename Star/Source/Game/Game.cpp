#include "Game.h"

#include "SceneSerializer.h"
#include "Engine/IGraphics.h"
#include "Engine/SceneManager.h"
#include "Engine/Implementation/Helpers.h"
#include "Engine/Implementation/Logging/Debug.h"

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


