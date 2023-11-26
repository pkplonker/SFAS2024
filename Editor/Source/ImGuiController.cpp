#include "ImGuiController.h"
#include <vector>

#include "FileDialog.h"
#include "Game.h"
#include "imgui.h"
#include "ImGuiTheme.h"

#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "SceneSerializer.h"
#include "Engine/Implementation/DirectX11/DirectX11Graphics.h"
#include "Windows/Hierarchy.h"
#include "Windows/ImGuiFPSCounter.h"
#include "Windows/Inspector.h"
#include "Windows/MeshImporterWindow.h"

ImGuiController::ImGuiController(DirectX11Graphics* dx11Graphics, Game* game) : dx11Graphics(dx11Graphics), game(game)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports;
    io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;

    ImGui_ImplWin32_Init(dx11Graphics->GetHWND());
    ImGui_ImplDX11_Init(dx11Graphics->GetDevice(), dx11Graphics->GetContext());
    const std::shared_ptr<ImGuiFPSCounter> fpsCounter = std::make_shared<ImGuiFPSCounter>();
    renderables.try_emplace(fpsCounter, true);
    const std::shared_ptr<Hierarchy> hierarchy = std::make_shared<Hierarchy>(game->GetScene());
    renderables.try_emplace(hierarchy, true);
    const std::shared_ptr<Inspector> inspector = std::make_shared<Inspector>(hierarchy);
    renderables.try_emplace(inspector, true);
    const std::shared_ptr<MeshImporterWindow> meshImporterWindow = std::make_shared<MeshImporterWindow>();
    renderables.try_emplace(meshImporterWindow, true);
    ImGuiTheme::ApplyTheme(0);
}

void ImGuiController::ImGuiPreFrame()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport();
    ImGui::ShowDemoWindow();
}


void ImGuiController::DrawViewport()
{
    ImGui::Begin("GameView");
    auto w = dx11Graphics->GetWidth();
    auto h = dx11Graphics->GetHeight();
    ImGui::Image((ImTextureID)dx11Graphics->GetTextureView(),
                 ImVec2(dx11Graphics->GetTextureWidth(), dx11Graphics->GetTextureHeight()));
    gameViewportSize = ImGui::GetWindowSize();
    ImGui::End();
}

void ImGuiController::DrawMenu()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Save"))
            {
                SceneSerializer::Serialize();
            }
            if (ImGui::MenuItem("Load"))
            {
                auto path = FileDialog::OpenFileDialog();
                if (path != "")
                {
                   SceneSerializer::Deserialize(path);
                }
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Windows"))
        {
            for (auto& item : renderables)
            {
                bool& isVisible = item.second;

                if (ImGui::MenuItem(item.first->GetName().c_str(), nullptr, isVisible))
                {
                    isVisible = !isVisible;
                }
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void ImGuiController::DrawWindows()
{
    std::vector<std::shared_ptr<EditorWindow>> identifiersToRemove;

    for (const auto& renderable : renderables)
    {
        // Todo make this better, second/first garbage
        if (renderable.first == nullptr)
        {
            identifiersToRemove.push_back(renderable.first);
        }
        else
        {
            if (renderable.second)
                renderable.first->Draw();
        }
    }

    for (const auto& identifier : identifiersToRemove)
    {
        renderables.erase(identifier);
    }
}

void ImGuiController::Draw()
{
    DrawWindows();
    DrawViewport();
    DrawMenu();
}

void ImGuiController::ImGuiPostUpdate() const
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
