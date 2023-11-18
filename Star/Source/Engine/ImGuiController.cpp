#include "ImGuiController.h"

#include <vector>

#include "IImGuiRenderable.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "Implementation/DirectX11/DirectX11Graphics.h"
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
    //ImGui::ShowDemoWindow();

    std::vector<std::string> identifiersToRemove;

    for (const auto& renderable : renderables)
    {
        if (renderable.second.first == nullptr)
        {
            identifiersToRemove.push_back(renderable.first);
        }
        else
        {
            if (renderable.second.second)
                renderable.second.first->Render(renderable.first);
        }
    }

    for (const auto& identifier : identifiersToRemove)
    {
        renderables.erase(identifier);
    }
    DrawMenu();
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

void ImGuiController::DrawMenu()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Windows"))
        {
            for (auto& item : renderables) {
                if (ImGui::MenuItem(item.first.c_str(), "", &item.second.second)) {
                }
            }

            ImGui::EndMenu();
        }
        
        ImGui::EndMainMenuBar();
    }
}

void ImGuiController::RegisterWindow(IImGuiRenderable* renderable, std::string identifier)
{
    renderables.try_emplace(identifier, std::make_pair(renderable, true));
}
