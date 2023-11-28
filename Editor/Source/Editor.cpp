#include "Editor.h"
#include "Engine/IGraphics.h"
#include "Engine/IInput.h"
#include "Engine/Implementation/Debug.h"
#include "Engine/ResourceManager.h"
#include "ImGuiController.h"
#include "MeshImporter.h"
#include "SceneSerializer.h"
#include "Engine/MeshSerializer.h"
#include "Engine/Implementation/DirectX11/DirectX11Graphics.h"

#define CLAMP(v, x, y) fmin(fmax(v, x), y)

constexpr float PieVal = 3.14159265359f;
constexpr float TwoPies = PieVal * 2.0f;
constexpr float DeltaTime = 0.016f;
constexpr float SpinSpeed = 0.1f;
constexpr float WinTolerance = PieVal / 10.0f;

IApplication* GetEditorApplication(IGraphics* Graphics, IInput* Input, HWND hwnd)
{
	return new Editor(Graphics, Input, hwnd);
}

Editor::Editor(IGraphics* GraphicsIn, IInput* InputIn, HWND hwnd) : IApplication(GraphicsIn, InputIn), hwnd(hwnd)
{
	game = new Game(Graphics, InputIn);
	dx11Graphics = dynamic_cast<DirectX11Graphics*>(Graphics);
}

Editor::~Editor()
{
}

bool Editor::IsValid()
{
	return true;
}

bool Editor::Load()
{
	game->Load();
	imguiController = std::make_unique<ImGuiController>(dx11Graphics, game);
	sceneSerializer = std::make_unique<SceneSerializer>(game->GetScene(),dx11Graphics);
	dx11Graphics->SetRenderToTexture(true, 1, 1);
	ResourceManager::Init(dx11Graphics);
	return true;
}

void Editor::Update()
{
	game->Update();

	imguiController->ImGuiPreFrame();

	imguiController->Draw();
}

void Editor::Cleanup()
{
	game->Cleanup();
	imguiController->ShutDown();
}

void Editor::PostGraphics()
{
	imguiController->ImGuiPostUpdate();
}