#include "ImGuiController.h"
#include <vector>

#include "Editor.h"
#include "EditorSettings.h"
#include "FileDialog.h"
#include "Game.h"
#include "imgui.h"
#include "ImGuiTheme.h"

#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "SceneManager.h"
#include "SceneSerializer.h"
#include "Engine/Implementation/DirectX11/DirectX11Graphics.h"
#include "Logging/BufferSink.h"
#include "Windows/Hierarchy.h"
#include "Windows/InputStatsWindow.h"
#include "Windows/TimeWindow.h"
#include "Windows/Inspector.h"
#include "Windows/LoggerWindow.h"
#include "Windows/MeshImporterWindow.h"
#include "Windows/RenderStatWindow.h"

const std::string IMGUI_SETTING_ID = "IMGUI_WINDOW";

ImGuiController::ImGuiController(DirectX11Graphics* dx11Graphics, Game* game, IInput* input) : dx11Graphics(dx11Graphics), game(game), input(input)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = "S:/Users/pkplo/OneDrive/Documents/C++/SFAS2024/Editor/Resource/imgui.ini";
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports;
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;

    ImGui_ImplWin32_Init(dx11Graphics->GetHWND());
    ImGui_ImplDX11_Init(dx11Graphics->GetDevice(), dx11Graphics->GetContext());
    const std::shared_ptr<TimeWindow> timeWindow = std::make_shared<TimeWindow>();
    renderables.try_emplace(timeWindow, EditorSettings::Get(IMGUI_SETTING_ID + timeWindow->GetName(), true));

    const std::shared_ptr<Hierarchy> hierarchy = std::make_shared<Hierarchy>();
    renderables.try_emplace(hierarchy, EditorSettings::Get(IMGUI_SETTING_ID + hierarchy->GetName(), true));

    const std::shared_ptr<Inspector> inspector = std::make_shared<Inspector>(hierarchy);
    renderables.try_emplace(inspector, EditorSettings::Get(IMGUI_SETTING_ID + inspector->GetName(), true));

    const std::shared_ptr<MeshImporterWindow> meshImporterWindow = std::make_shared<MeshImporterWindow>();
    renderables.try_emplace(meshImporterWindow,
                            EditorSettings::Get(IMGUI_SETTING_ID + meshImporterWindow->GetName(), true));

    const std::shared_ptr<RenderStatWindow> drawStats = std::make_shared<RenderStatWindow>();
    renderables.try_emplace(drawStats, EditorSettings::Get(IMGUI_SETTING_ID + drawStats->GetName(), true));
    
    bufferSink = new BufferSink(1000);
    const std::shared_ptr<LoggerWindow> logger = std::make_shared<LoggerWindow>(bufferSink);
    renderables.try_emplace(logger, EditorSettings::Get(IMGUI_SETTING_ID + logger->GetName(), true));

    const std::shared_ptr<InputStatsWindow> inputWindow = std::make_shared<InputStatsWindow>(input);
    renderables.try_emplace(inputWindow, EditorSettings::Get(IMGUI_SETTING_ID + inputWindow->GetName(), true));

    
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
    ImGui::Begin("GameView", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    auto w = dx11Graphics->GetWidth();
    auto h = dx11Graphics->GetHeight();
    ImGui::Image((ImTextureID)dx11Graphics->GetTextureView(),
                 ImVec2(static_cast<float>(dx11Graphics->GetTextureWidth()),
                        static_cast<float>(dx11Graphics->GetTextureHeight())));
    gameViewportSize = ImGui::GetWindowSize();
    ImGui::End();
}

void ImGuiController::AddWindow(const std::shared_ptr<EditorWindow>& shared)
{
    renderables.try_emplace(shared, EditorSettings::Get(IMGUI_SETTING_ID + shared->GetName(), true));
}


void ImGuiController::ImGuiPostUpdate() const
{
    ImGui::Render();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    dx11Graphics->SetRenderToTexture(true, gameViewportSize.x, gameViewportSize.y);
    if (auto scene = SceneManager::GetScene().lock())
    {
        if (scene->GetActiveCamera() != nullptr)
        {
            scene->GetActiveCamera()->SetWidth(gameViewportSize.x);
            scene->GetActiveCamera()->SetWidth(gameViewportSize.y);
        }
    }
}

void ImGuiController::Save()
{
    SceneSerializer::Serialize(FileDialog::SaveFileDialog());
}

void ImGuiController::LoadScene() const
{
    LoadScene(FileDialog::OpenFileDialog());
}

void ImGuiController::LoadScene(std::string path) const
{
    if (path != "")
    {
        SceneManager::SetScene(SceneSerializer::Deserialize(path));
        EditorSettings::Set("LastScene", path);
    }
}

void ImGuiController::DrawMenu()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Save", "Ctrl+S"))
            {
                Save();
            }
            if (ImGui::MenuItem("Load", "Ctrl+O"))
            {
                LoadScene();
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
        if (ImGui::BeginMenu("Tools"))
        {
            if (ImGui::MenuItem("Import Mesh"))
            {
                MeshImporterWindow::Load();
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


void ImGuiController::ShutDown()
{
    for (auto renderable : renderables)
    {
        EditorSettings::Set(IMGUI_SETTING_ID + renderable.first->GetName(), renderable.second);
    }
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    Debug::DeregisterSink(bufferSink);
    delete bufferSink;
}

void ImGuiController::Resize(int width, int height)
{
    RECT rect;
    ::GetClientRect(dx11Graphics->GetHWND(), &rect);
    float x = static_cast<float>(rect.right - rect.left);
    float y = static_cast<float>(rect.bottom - rect.top);
    ImGui::GetIO().DisplaySize = ImVec2(x, y);
    Trace(std::to_string(x) + ":" + std::to_string(y))
}
