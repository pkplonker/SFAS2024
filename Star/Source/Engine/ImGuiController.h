#pragma once
#include <string>

#include "Implementation/DirectX11/DirectX11Graphics.h"
#include "Engine/IUpdatePipe.h"

class IImGuiRenderable;

class ImGuiController : public IUpdatePipe
{
public:
    ImGuiController() = default;
    void Init(DirectX11Graphics* Graphics, IInput* Input);
    void PreUpdate() override;

    void Update() override;
    void PostUpdate() override;

    void ShutDown() override;
    void DrawMenu();
    void RegisterWindow(IImGuiRenderable* renderable, std::string identifier);
    //todo serialize window state
private:
    std::map<std::string, std::pair<IImGuiRenderable*, bool>> renderables;
};
