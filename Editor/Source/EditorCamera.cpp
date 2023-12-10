#include "EditorCamera.h"

#include <iostream>

#include "IInput.h"
#include "PerspectiveCamera.h"
#include "SceneManager.h"
#include "Transform3D.h"
#include "Implementation/Scene.h"

EditorCamera::EditorCamera(IInput* input)
{
    this->input = input;
    transform = std::make_shared<Transform3D>();
    transform->Position = Vec3(0, 2.5, -12);
    camera = std::make_shared<PerspectiveCamera>(transform, 1000, 1000);
    camera->SetFOV(80);
}

void EditorCamera::SetActiveCamera()
{
    if (auto scene = SceneManager::GetScene().lock())
    {
        scene->SetActiveCamera(shared_from_this());
    }
}

std::shared_ptr<Transform3D> EditorCamera::GetTransform()
{
    return transform;
}

void EditorCamera::SetTransform(std::shared_ptr<Transform3D> transform)
{
    this->transform = transform;
}

DirectX::XMMATRIX EditorCamera::GetProjectionMatrix()
{
    return camera->GetProjectionMatrix();
}

DirectX::XMMATRIX EditorCamera::GetViewMatrix()
{
    return camera->GetViewMatrix();
}

DirectX::XMMATRIX EditorCamera::GetViewProjectionMatrix()
{
    return camera->GetViewProjectionMatrix();
}

void EditorCamera::SetHeight(float height)
{
    camera->SetHeight(height);
}

void EditorCamera::SetWidth(float width)
{
    camera->SetWidth(width);
}

void EditorCamera::SetFov(int fov)
{
    camera->SetFOV(fov);
}

void EditorCamera::Update()
{
    auto deltaTime = 0.16f;

    DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(
        DirectX::XMConvertToRadians(transform->Rotation.X()),
        DirectX::XMConvertToRadians(transform->Rotation.Y()),
        DirectX::XMConvertToRadians(transform->Rotation.Z()));

    if (input->IsRightHeld())
    {
        transform->Rotation = Vec3(transform->Rotation.X() + input->GetDeltaY() * xRotSpeed * deltaTime,
                                   transform->Rotation.Y() + input->GetDeltaX() * yRotSpeed * deltaTime,
                                   transform->Rotation.Z());
    }

    DirectX::XMFLOAT3 moveVec(0.0f, 0.0f, 0.0f);

    if (input->IsMiddleHeld())
    {
        moveVec.x = input->GetDeltaX() * -xMoveSpeed * deltaTime;
        moveVec.y = input->GetDeltaY() * yMoveSpeed * deltaTime;
    }

    int scroll = input->GetMouseScrollDelta();
    if (scroll != 0)
    {
        moveVec.z = scroll * scrollSpeed * deltaTime;
    }

    if (moveVec.x != 0.0f || moveVec.y != 0.0f || moveVec.z != 0.0f)
    {
        DirectX::XMVECTOR moveVecDX = DirectX::XMLoadFloat3(&moveVec);
        moveVecDX = DirectX::XMVector3TransformNormal(moveVecDX, rotationMatrix);
        DirectX::XMStoreFloat3(&moveVec, moveVecDX);

        transform->Position = Vec3(transform->Position.X() + moveVec.x,
                                   transform->Position.Y() + moveVec.y,
                                   transform->Position.Z() + moveVec.z);
    }
}
