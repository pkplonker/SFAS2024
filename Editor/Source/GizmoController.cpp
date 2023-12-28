#include "GizmoController.h"

#include <iostream>

#include "EditorCamera.h"
#include "ImGuizmo.h"
#include "Transform.h"

GizmoController::GizmoController(std::weak_ptr<EditorCamera> editorCamera) : editorCamera(editorCamera)
{
}

void GizmoController::ImGuiPreFrame(ImVec2 size, ImVec2 position)
{
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::BeginFrame();
}

void GizmoController::Update(std::weak_ptr<GameObject> gameobject, ImVec2 size, ImVec2 position)
{
    ImGuizmo::Enable(true);
    ImGuizmo::SetRect(position.x, position.y, size.x, size.y);

    ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
    ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);

    if (const auto& cam = editorCamera.lock())
    {
        if (const auto& object = gameobject.lock())
        {
            DirectX::XMFLOAT4X4 v = ChangeMat(cam->GetViewMatrix());
            DirectX::XMFLOAT4X4 p = ChangeMat(cam->GetProjectionMatrix());
            DirectX::XMFLOAT4X4 w = ChangeMat(object->Transform()->GetWorldMatrix());


            ImGuizmo::Manipulate(&v.m[0][0], &p.m[0][0],
                                 mCurrentGizmoOperation, mCurrentGizmoMode, &w.m[0][0]);


            if (ImGuizmo::IsUsing())
            {
                object->Transform()->SetWorldMatrix(&w.m[0][0]);
            }
        }
    }
}

DirectX::XMFLOAT4X4 GizmoController::ChangeMat(DirectX::XMMATRIX mat)
{
    DirectX::XMFLOAT4X4 temp;
    DirectX::XMStoreFloat4x4(&temp, mat);

    return temp;
}
