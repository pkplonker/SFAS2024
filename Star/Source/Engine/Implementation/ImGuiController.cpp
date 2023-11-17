#include "ImGuiController.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "DirectX11/DirectX11Graphics.h"
#include "Engine/IUpdatePipe.h"

void ImGuiController::Init(DirectX11Graphics* Graphics, IInput* Input)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    ImGui_ImplWin32_Init(Graphics->GetHWND());
    ImGui_ImplDX11_Init(Graphics->GetDevice(), Graphics->GetContext());
}

void ImGuiController::PreUpdate()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void ImGuiController::Update()
{
    ImGui::ShowDemoWindow();
    ImGui::Begin("Hello world");
    ImGui::Text("Test test test");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                ImGui::GetIO().Framerate);
    ImGui::End();
    
}

void ImGuiController::PostUpdate()
{
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiController::ShutDown()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}
