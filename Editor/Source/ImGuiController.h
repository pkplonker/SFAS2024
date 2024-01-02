#pragma once
#include <map>
#include <memory>
#include <xstring>

#include "GizmoController.h"
#include "IInput.h"
#include "imgui.h"
#include "IResizeHandler.h"
#include "Logging/BufferSink.h"
#include "Windows/Hierarchy.h"

class SettingsWindow;
class Game;
class EditorWindow;
class DirectX11Graphics;
class GizmoController;
class EditorCamera;

class ImGuiController : public IResizeHandler
{
public:
    void LoadIconFonts();
    static ImFont* LargeIcons() { return largeIcons; }
    static ImFont* SmallIcons() { return smallIcons; }

    ImGuiController(DirectX11Graphics* dx11Graphics, Game* game, IInput* input, std::shared_ptr<EditorCamera> camera);
    ~ImGuiController() override;
    std::weak_ptr<GameObject> GetSelectedObject();
    void ImGuiPreFrame();
    void ViewPortActiveWindowCheck();
    void LoadScene();
    void LoadScene(std::string path);
    void New();
    void DrawMenu();
    void DrawViewport();
    void AddWindow(const std::shared_ptr<EditorWindow>& window);
    void SetSelectedObject(const std::shared_ptr<GameObject>& obj);

    bool IsUsingGizmo()
    {
        return isUsingGizmo;
    }

    static void Save();
    void Save(std::string path);
    void SaveExistingScene();
    void DrawWindows();
    void Draw();
    void ImGuiPostUpdate() const;
    void ShutDown();
    void Resize(int width, int height) override;

    ImVec2 GetViewportPosition()
    {
        return gameViewportPosition;
    }

    ImVec2 GetViewportSize()
    {
        return gameViewportSize;
    }

    bool IsViewportInFocus()
    {
        return viewportFocused;
    }
    static float GetSmallFontSize(){return smallFontSize;}
    static float GetLargeFontSize(){return largeFontSize;}

private:
    DirectX11Graphics* dx11Graphics;
    Game* game;
    ImVec2 gameViewportSize;
    std::map<std::shared_ptr<EditorWindow>, bool> renderables;
    BufferSink* bufferSink;
    IInput* input;
    bool openSettings;
    SettingsWindow* settingsWindow;
    bool viewportFocused;
    std::shared_ptr<Hierarchy> hierarchy;
    ImVec2 gameViewportPosition;
    std::shared_ptr<EditorCamera> camera;
    std::shared_ptr<GizmoController> gizmoController;
    bool isUsingGizmo;
    static inline constexpr float smallFontSize = 13 * 1.3f;
    static inline constexpr float largeFontSize = 13 * 2.0f;
    static inline ImFont* largeIcons;
    static inline ImFont* smallIcons;
};
