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

IApplication* GetEditorApplication(IGraphics* Graphics, IInput* Input, HWND hwnd)
{
	return new Editor(Graphics, Input, hwnd);
}

Editor::Editor(IGraphics* GraphicsIn, IInput* InputIn, HWND hwnd) : IApplication(GraphicsIn, InputIn), hwnd(hwnd)
{
	dx11Graphics = dynamic_cast<DirectX11Graphics*>(Graphics);
	gameGraphics = new DirectX11Graphics(hwnd);

	game = new Game(gameGraphics, InputIn);
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

	game->Load();
	gameGraphics->SetRenderToTexture(true);
	gameGraphics->GetContext()->Flush();
	IDXGIResource* dxgiResource = nullptr;
	auto hr = gameGraphics->GetTexture()->QueryInterface(__uuidof(IDXGIResource), (void**)&dxgiResource);
	if (FAILED(hr)) {
		Debug("failed")
	}

	dxgiResource->GetSharedHandle(&gameRenderTextureHandle);
	dxgiResource->Release();

	return true;
}

void Editor::Update()
{
	game->Update();
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::ShowDemoWindow();

	gameGraphics->Update();
	gameGraphics->PostUpdate();
	dx11Graphics->GetContext()->Flush();
	gameGraphics->GetContext()->Flush();

	ID3D11Texture2D* openedTexture = nullptr;
	auto hr = dx11Graphics->GetDevice()->OpenSharedResource(gameRenderTextureHandle, __uuidof(ID3D11Texture2D), (void**)&openedTexture);
	if (FAILED(hr)) {
		Debug("failed")
	}
	ImTextureID tex_id = (ImTextureID)gameGraphics->GetTextureView();

	ImGui::Begin("GameView");
	auto w = gameGraphics->GetWidth();
	auto h = gameGraphics->GetHeight();
	ImGui::Image(tex_id, ImVec2(gameGraphics->GetWidth(), gameGraphics->GetHeight()));

	//ImGuiIO& io = ImGui::GetIO();

	//ImTextureID my_tex_id = io.Fonts->TexID;
	//ImVec2 pos = ImGui::GetCursorScreenPos();
	//ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
	//ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
	//ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	//ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
	//float my_tex_w = (float)io.Fonts->TexWidth;
	//float my_tex_h = (float)io.Fonts->TexHeight;
	//ImGui::Image(my_tex_id, ImVec2(my_tex_w, my_tex_h), uv_min, uv_max, tint_col, border_col);

	ImGui::End();
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
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}