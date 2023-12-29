#pragma once

#include <memory>

#include "EditorCamera.h"
#include "Game.h"
#include "imgui.h"
#include "SceneSerializer.h"
#include "Engine/IApplication.h"
#include "Engine/Implementation/Scene.h"
#include "ImGuiController.h"

class EditorViewportInteractor;
class UndoManager;
class DirectX11Graphics;
class ResourceManager;
class GameObject;
class IGraphics;
class ITexture;
class IShader;
class IRenderable;
struct HWND__;
typedef struct HWND__* HWND;
typedef void* HANDLE;

class Editor : public IApplication
{
public:
    Editor(IGraphics* Graphics, IInput* InputIn, HWND hwnd);
    virtual ~Editor();

    virtual bool IsValid();
    virtual bool Load();
    virtual void Update();
    virtual void Cleanup();
    virtual void PostGraphics();
    static IGraphics* GetGraphics();
    void Resize(int width, int height) override;

private:
    inline static std::shared_ptr<GameObject> camera = {};
    inline static DirectX11Graphics* dx11Graphics = {};
    inline static Game* game = {};
    inline static HWND hwnd = {};
    inline static ImVec2 gameViewportSize = {};
    inline static std::unique_ptr<ImGuiController> imguiController = {};
    inline static std::unique_ptr<SceneSerializer> sceneSerializer = {};
    std::shared_ptr<EditorCamera> editorCamera;
    std::unique_ptr<UndoManager> undomanager;
    std::unique_ptr<EditorViewportInteractor> editorViewportInteractor;
};
