#include "Editor.h"
#include "Engine/IGraphics.h"
#include "Engine/IInput.h"
#include "Engine/Implementation/Debug.h"
#include "Engine/ResourceManager.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
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
	dx11Graphics = dynamic_cast<DirectX11Graphics*>(Graphics);
	dx11Graphics->SetRenderToTexture(true, 900, 600);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports;
	io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;

	ImGui_ImplWin32_Init(dx11Graphics->GetHWND());
	ImGui_ImplDX11_Init(dx11Graphics->GetDevice(), dx11Graphics->GetContext());

	return true;
}

void Editor::Update()
{
	game->Update();

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::DockSpaceOverViewport();

	ImGui::ShowDemoWindow();
	dx11Graphics->GetContext()->Flush();
	ImTextureID tex_id = (ImTextureID)dx11Graphics->GetTextureView();

	ImGui::Begin("GameView");
	auto w = dx11Graphics->GetWidth();
	auto h = dx11Graphics->GetHeight();
	ImGui::Image(tex_id, ImVec2(dx11Graphics->GetTextureWidth(), dx11Graphics->GetTextureHeight()));
	ImVec2 windowSize = ImGui::GetWindowSize();

	ImGui::End();
	//dx11Graphics->SetRenderToTexture(true, windowSize.x, windowSize.y);
}

void Editor::Cleanup()
{
	game->Cleanup();
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Editor::PostGraphics()
{
	ImGui::Render();
	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}