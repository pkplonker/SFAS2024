#include "GizmoController.h"

#include <iostream>

#include "EditorCamera.h"
#include "ImGuizmo.h"
#include "Transform.h"

GizmoController::GizmoController(std::weak_ptr<EditorCamera> editorCamera) : editorCamera(editorCamera)
{
}

void GizmoController::ImGuiPreFrame(Vec2 size)
{
    // ImGuizmo::SetOrthographic(false);
    // ImGuizmo::SetRect(0, 0, size.X(), size.Y());
    // ImGuizmo::BeginFrame();
}
void GizmoController::Update(std::weak_ptr<GameObject> transform)
{
}
