#pragma once
#include <DirectXMath.h>

#include "CameraComponent.h"
#include "imgui.h"
#include "IUpdateable.h"
#include "Math/Vector3.h"

class ImGuiController;
class IInput;

class EditorViewportInteractor
{
public:
    EditorViewportInteractor(IInput* input, ImGuiController* controller);
    void Update(std::weak_ptr<ICamera> cameraComponent);
    void ScreenPointToRay(Vec2 mousePos, int viewportWidth, int viewportHeight, std::weak_ptr<ICamera> cameraComponent, DirectX::XMVECTOR
                          & rayOrigin, DirectX::XMVECTOR& rayDirection, ImVec2 viewportPosition);

private:
    ImGuiController* controller;
    IInput* input;
};
