#include "Editor.h"

#include "DebugDrawer.h"
#include "EditorCamera.h"
#include "EditorSettings.h"
#include "EditorViewportInteractor.h"
#include "Helpers.h"
#include "Engine/IGraphics.h"
#include "Engine/IInput.h"
#include "Engine/Implementation/Logging/Debug.h"
#include "Engine/ResourceManager.h"
#include "ImGuiController.h"
#include "MeshImporter.h"
#include "SceneManager.h"
#include "SceneSerializer.h"
#include "UndoManager.h"
#include "Engine/Implementation/DirectX11/DirectX11Graphics.h"
#include "Windows/EditorCameraWindow.h"
#include "Windows/EditorWindow.h"

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

Editor::Editor(IGraphics* GraphicsIn, IInput* InputIn, HWND hwnd) : IApplication(GraphicsIn, InputIn)
{
    Editor::hwnd = hwnd;
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

    sceneSerializer = std::make_unique<SceneSerializer>(dx11Graphics);
    dx11Graphics->SetRenderToTexture(true, 1, 1);
    ResourceManager::Init(dx11Graphics);
    imguiController->LoadScene(
        EditorSettings::Get("LastScene",
                            Helpers::WideStringToString(
                                L"S:/Users/pkplo/OneDrive/Documents/C++/SFAS2024/Editor/Resource/Scenes/TestScene2.scene")));


    SceneManager::OnSceneChangedEvent.Subscribe([this]()
    {
        if (auto scene = SceneManager::GetScene().lock())
        {
            scene->SetActiveCamera(editorCamera);
        }
    });
    undomanager = std::make_unique<UndoManager>(Input);
    editorCamera = std::make_shared<EditorCamera>(Input);
    editorCamera->SetActiveCamera();
    imguiController = std::make_unique<ImGuiController>(dx11Graphics, game, Input, editorCamera);
    imguiController->AddWindow(std::make_shared<EditorCameraWindow>(editorCamera));
    editorViewportInteractor = std::make_unique<EditorViewportInteractor>(Input, imguiController.get());
    debugDrawer = std::make_shared<DebugDrawer>(editorCamera);
    Graphics->AddRenderStrategy(debugDrawer);
    //game->Load();

    return true;
}

void Editor::Update()
{
    game->Update();
    undomanager->Update();
    imguiController->ImGuiPreFrame();

    editorCamera->Update(imguiController->IsViewportInFocus(), imguiController->GetSelectedObject());

    imguiController->Draw();
    if (!imguiController->IsUsingGizmo())
    {
        editorViewportInteractor->Update(editorCamera->camera);
    }
}

void Editor::Cleanup()
{
    if (const auto s = SceneManager::GetScene().lock())
    {
        const auto& path = s->GetPath();
        EditorSettings::Set("LastScene", path);
        imguiController->SaveExistingScene();
        
    }
    EditorSettings::SaveSettings();

    game->Cleanup();
    imguiController->ShutDown();
}

void Editor::PostGraphics()
{
    imguiController->ImGuiPostUpdate();
}

IGraphics* Editor::GetGraphics()
{
    return Editor::dx11Graphics;
}

void Editor::Resize(int width, int height)
{
    Graphics->Resize(width, height);
    imguiController->Resize(width, height);
}
