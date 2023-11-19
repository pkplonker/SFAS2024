#include "Editor.h"

#include "Engine/IGraphics.h"
#include "Engine/IRenderable.h"
#include "Engine/IInput.h"
#include <ctime>

#include "Engine/Implementation/Debug.h"
#include "Engine/Implementation/GameObject.h"
#include "Engine/Implementation/GameObjectFactory.h"
#include "Engine/ResourceManager.h"
#include "Engine/Implementation/CameraComponent.h"
#include "Engine/ImGuiController.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#define CLAMP(v, x, y) fmin(fmax(v, x), y)

constexpr float PieVal = 3.14159265359f;
constexpr float TwoPies = PieVal * 2.0f;
constexpr float DeltaTime = 0.016f;
constexpr float SpinSpeed = 0.1f;
constexpr float WinTolerance = PieVal / 10.0f;

IApplication* GetApplication(IGraphics* Graphics, IInput* Input)
{
	return new Editor(Graphics, Input);
}

Editor::Editor(IGraphics* GraphicsIn, IInput* InputIn) : IApplication(GraphicsIn, InputIn)
{
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
	DirectX11Graphics* dx11Graphics = dynamic_cast<DirectX11Graphics*>(Graphics);
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	ImGui_ImplWin32_Init(dx11Graphics->GetHWND());
	ImGui_ImplDX11_Init(dx11Graphics->GetDevice(), dx11Graphics->GetContext());

	return true;
}

void Editor::Update()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::ShowDemoWindow();
}

void Editor::Cleanup()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Editor::PostGraphics()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}