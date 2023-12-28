#include "ImGuiController.h"
#include <vector>

#include "Editor.h"
#include "EditorSettings.h"
#include "FileDialog.h"
#include "Game.h"
#include "GizmoController.h"
#include "imgui.h"
#include "ImGuiTheme.h"
#include "ImGuizmo.h"

#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "imgui_internal.h"
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
#include "Windows/ObjectControlWindow.h"
#include "Windows/RenderStatWindow.h"
#include "Windows/SettingsWindow.h"
#include "Windows/UndoWindow.h"

const std::string IMGUI_SETTING_ID = "IMGUI_WINDOW";

ImGuiController::ImGuiController(DirectX11Graphics* dx11Graphics, Game* game, IInput* input,
                                 std::shared_ptr<EditorCamera> camera) :
    dx11Graphics(dx11Graphics), game(game), input(input), camera(camera)
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

    hierarchy = std::make_shared<Hierarchy>();
    renderables.try_emplace(hierarchy, EditorSettings::Get(IMGUI_SETTING_ID + hierarchy->GetName(), true));

    const std::shared_ptr<Inspector> inspector = std::make_shared<Inspector>(hierarchy);
    renderables.try_emplace(inspector, EditorSettings::Get(IMGUI_SETTING_ID + inspector->GetName(), true));

    const std::shared_ptr<MeshImporterWindow> meshImporterWindow = std::make_shared<MeshImporterWindow>();
    renderables.try_emplace(meshImporterWindow,
                            EditorSettings::Get(IMGUI_SETTING_ID + meshImporterWindow->GetName(), true));

    const std::shared_ptr<RenderStatWindow> drawStats = std::make_shared<RenderStatWindow>();
    renderables.try_emplace(drawStats, EditorSettings::Get(IMGUI_SETTING_ID + drawStats->GetName(), true));

    bufferSink = new BufferSink(1000);
    Debug::RegisterSink(bufferSink);
    Trace("Registered logging")

    gizmoController = std::make_shared<GizmoController>(camera, input);

    const std::shared_ptr<LoggerWindow> logger = std::make_shared<LoggerWindow>(bufferSink);
    renderables.try_emplace(logger, EditorSettings::Get(IMGUI_SETTING_ID + logger->GetName(), true));

    const std::shared_ptr<InputStatsWindow> inputWindow = std::make_shared<InputStatsWindow>(input);
    renderables.try_emplace(inputWindow, EditorSettings::Get(IMGUI_SETTING_ID + inputWindow->GetName(), true));

    const std::shared_ptr<UndoWindow> undoWindow = std::make_shared<UndoWindow>();
    renderables.try_emplace(undoWindow, EditorSettings::Get(IMGUI_SETTING_ID + undoWindow->GetName(), true));

    const std::shared_ptr<ObjectControlWindow> objectControl = std::make_shared<ObjectControlWindow>(gizmoController);
    renderables.try_emplace(objectControl, EditorSettings::Get(IMGUI_SETTING_ID + objectControl->GetName(), true));

    settingsWindow = new SettingsWindow();

    ImGuiTheme::ApplyTheme(0);
    Trace("Imgui setup complete")
}

ImGuiController::~ImGuiController()
{
    delete settingsWindow;
}

std::weak_ptr<GameObject> ImGuiController::GetSelectedObject()
{
    return hierarchy->GetSelectedObject();
}

void ImGuiController::ImGuiPreFrame()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    gizmoController->ImGuiPreFrame();
    ImGui::DockSpaceOverViewport();
    ImGui::ShowDemoWindow();
}

void ImGuiController::ViewPortActiveWindowCheck()
{
    viewportFocused = ImGui::IsWindowHovered();
}

void ImGuiController::DrawViewport()
{
    ImGui::Begin("GameView", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    ViewPortActiveWindowCheck();

    ImGui::Image((ImTextureID)dx11Graphics->GetTextureView(),
                 ImVec2(static_cast<float>(dx11Graphics->GetTextureWidth()),
                        static_cast<float>(dx11Graphics->GetTextureHeight())));
    gameViewportSize = ImGui::GetWindowSize();
    gameViewportPosition = ImGui::GetWindowPos();


    gizmoController->Update(IsViewportInFocus(), GetSelectedObject(), gameViewportSize, gameViewportPosition);


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
    dx11Graphics->SetRenderToTexture(true, static_cast<int>(gameViewportSize.x), static_cast<int>(gameViewportSize.y));
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
        if (ImGui::MenuItem("Settings"))
        {
            openSettings = true;
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

    if (openSettings)
    {
        ImGui::OpenPopup("Settings");
        openSettings = false;
    }
    settingsWindow->Draw();
}

void ImGuiController::DrawWindows()
{
    std::vector<std::shared_ptr<EditorWindow>> identifiersToRemove;

    for (const auto [window,visible] : renderables)
    {
        if (window == nullptr)
        {
            identifiersToRemove.push_back(window);
        }
        else
        {
            if (visible)
                window->Draw();
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
    const auto x = static_cast<float>(rect.right - rect.left);
    const auto y = static_cast<float>(rect.bottom - rect.top);
    ImGui::GetIO().DisplaySize = ImVec2(x, y);
    Trace(std::to_string(x) + ":" + std::to_string(y))
}
