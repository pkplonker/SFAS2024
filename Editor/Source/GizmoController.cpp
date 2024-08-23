#include "GizmoController.h"

#include <iostream>

#include "EditorCamera.h"
#include "ImGuizmo.h"
#include "Transform.h"
#include "UndoManager.h"

GizmoController::GizmoController(std::weak_ptr<EditorCamera> editorCamera, IInput* input) : editorCamera(editorCamera),
    input(input)
{
}

void GizmoController::ImGuiPreFrame()
{
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::BeginFrame();
}

void GizmoController::HandleInput(bool isInFocus)
{
    if (isInFocus && !(input->IsRightHeld() || input->IsRightPressed()))
    {
        if (input->IsKeyPress(Keys::W))
        {
            operation = Translation;
        }
        else if (input->IsKeyPress(Keys::E))
        {
            operation = Rotation;
        }
        else if (input->IsKeyPress(Keys::R))
        {
            operation = Scale;
        }
    }
}

bool GizmoController::Update(bool isInFocus, std::weak_ptr<GameObject> gameobject, ImVec2 size, ImVec2 position)
{
    bool isUsing = false;
    HandleInput(isInFocus);

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
        DirectX::XMFLOAT4X4 v = ChangeMat(cam->GetViewMatrix());
        DirectX::XMFLOAT4X4 p = ChangeMat(cam->GetProjectionMatrix());

        if (const auto& object = gameobject.lock())
        {
            DirectX::XMFLOAT4X4 w = ChangeMat(object->Transform()->GetWorldMatrix());
            DirectX::XMFLOAT4X4 d;
            ImGuizmo::Manipulate(&v.m[0][0], &p.m[0][0],
                                 currentOperation, currentMode, &w.m[0][0], &d.m[0][0]);

            isUsing = ImGuizmo::IsUsing();

            if (!wasUsing && isUsing)
            {
                lastInitialMatrix = object->Transform()->GetWorldMatrix();
            }

            if (isUsing)
            {
                DirectX::XMMATRIX deltaMatrix = DirectX::XMMATRIX(&d.m[0][0]);
                DirectX::XMVECTOR deltaTranslation, deltaRotationQuat, deltaScale;
                Vec3 translationChange;
                Vec3 rotationChange;
                Vec3 scaleChange;

                DirectX::XMMatrixDecompose(&deltaScale, &deltaRotationQuat, &deltaTranslation, deltaMatrix);
                switch (operation)
                {
                case Translation:
                    translationChange.X(DirectX::XMVectorGetX(deltaTranslation));
                    translationChange.Y(DirectX::XMVectorGetY(deltaTranslation));
                    translationChange.Z(DirectX::XMVectorGetZ(deltaTranslation));
                    object->Transform()->Translate(translationChange);
                    break;
                case Rotation:
                    DirectX::XMFLOAT3 eulerRotationRadians;
                    DirectX::XMStoreFloat3(&eulerRotationRadians,
                                           DirectX::XMQuaternionRotationRollPitchYawFromVector(deltaRotationQuat));

                    rotationChange.X(eulerRotationRadians.x * 180.0f / DirectX::XM_PI*4);
                    rotationChange.Y(eulerRotationRadians.y * 180.0f / DirectX::XM_PI*4);
                    rotationChange.Z(eulerRotationRadians.z * 180.0f / DirectX::XM_PI*4);

                    object->Transform()->Rotate(rotationChange);
                    break;
                case Scale:
                    scaleChange.X(DirectX::XMVectorGetX(deltaScale));
                    scaleChange.Y(DirectX::XMVectorGetY(deltaScale));
                    scaleChange.Z(DirectX::XMVectorGetZ(deltaScale));
                    
                    object->Transform()->ApplyScale(scaleChange);
                    break;
                default: ;
                }
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
    return ImGuizmo::IsUsing();
}


DirectX::XMFLOAT4X4 GizmoController::ChangeMat(DirectX::XMMATRIX mat)
{
    DirectX::XMFLOAT4X4 temp = {};
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
