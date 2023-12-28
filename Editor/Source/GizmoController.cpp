#include "GizmoController.h"

#include <iostream>

#include "EditorCamera.h"
#include "ImGuizmo.h"
#include "Transform.h"
#include "UndoManager.h"

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
    static DirectX::XMMATRIX lastInitialMatrix;

    ImGuizmo::OPERATION currentOperation = operation == Translation
                                               ? ImGuizmo::OPERATION::TRANSLATE
                                               : operation == Rotation
                                               ? ImGuizmo::OPERATION::ROTATE
                                               : ImGuizmo::OPERATION::SCALE;
    ImGuizmo::MODE currentMode = space == World ? ImGuizmo::MODE::WORLD : ImGuizmo::MODE::LOCAL;

    static bool wasUsing = false;
    if (const auto& cam = editorCamera.lock())
    {
        if (const auto& object = gameobject.lock())
        {
            DirectX::XMFLOAT4X4 v = ChangeMat(cam->GetViewMatrix());
            DirectX::XMFLOAT4X4 p = ChangeMat(cam->GetProjectionMatrix());
            DirectX::XMFLOAT4X4 w = ChangeMat(object->Transform()->GetWorldMatrix());

            ImGuizmo::Manipulate(&v.m[0][0], &p.m[0][0],
                                 currentOperation, currentMode, &w.m[0][0]);

            bool isUsing = ImGuizmo::IsUsing();

            if (!wasUsing && isUsing)
            {
                lastInitialMatrix = object->Transform()->GetWorldMatrix();
            }

            if (isUsing)
            {
                object->Transform()->SetWorldMatrix(&w.m[0][0]);
            }

            if (wasUsing && !isUsing)
            {
                DirectX::XMMATRIX finalMatrix = object->Transform()->GetWorldMatrix();

                DirectX::XMMATRIX operationInitialMatrix = lastInitialMatrix;

                UndoManager::Execute(Memento(
                                         [object, finalMatrix]()
                                         {
                                             object->Transform()->SetWorldMatrix(finalMatrix);
                                         },
                                         [object, operationInitialMatrix]()
                                         {
                                             object->Transform()->SetWorldMatrix(operationInitialMatrix);
                                         },
                                         "Modified transform"
                                     ), false);
            }

            wasUsing = isUsing;
        }
    }
}


DirectX::XMFLOAT4X4 GizmoController::ChangeMat(DirectX::XMMATRIX mat)
{
    DirectX::XMFLOAT4X4 temp;
    DirectX::XMStoreFloat4x4(&temp, mat);

    return temp;
}

void GizmoController::SetGizmoOpertion(GizmoOperation op)
{
    operation = op;
}

GizmoOperation GizmoController::GetGizmoOperation()
{
    return operation;
}

void GizmoController::SetMode(GizmoSpace mode)
{
    space = mode;
}

GizmoSpace GizmoController::GetMode()
{
    return space;
}
