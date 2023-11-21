#include "ImGuiController.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "Engine/Implementation/DirectX11/DirectX11Graphics.h"

ImGuiController::ImGuiController(DirectX11Graphics* dx11Graphics) :dx11Graphics(dx11Graphics)
{
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
}

void ImGuiController::ImGuiPreFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::DockSpaceOverViewport();
	ImGui::ShowDemoWindow();
}

void ImGuiController::ImGuiDrawViewport()
{
	ImGui::Begin("GameView");
	auto w = dx11Graphics->GetWidth();
	auto h = dx11Graphics->GetHeight();
	ImGui::Image((ImTextureID)dx11Graphics->GetTextureView(), ImVec2(dx11Graphics->GetTextureWidth(), dx11Graphics->GetTextureHeight()));
	gameViewportSize = ImGui::GetWindowSize();
	ImGui::End();
}
void ImGuiController::ImGuiPostUpdate()
{
	ImGui::Render();
	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	dx11Graphics->SetRenderToTexture(true, gameViewportSize.x, gameViewportSize.y);
}

void ImGuiController::ShutDown()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}