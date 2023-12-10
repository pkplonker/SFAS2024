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
    auto xRotSpeed = 1.0f;
    auto yRotSpeed = 1.0f;
    auto xMoveSpeed = 1.0f;
    auto yMoveSpeed = 1.0f;
    auto deltaTime = 0.16f;

    if (input->IsRightHeld())
    {
        transform->Rotation = Vec3(transform->Rotation.X() + input->GetDeltaY() * xRotSpeed * deltaTime,
                                   transform->Rotation.Y() + input->GetDeltaX() * yRotSpeed * deltaTime,
                                   transform->Rotation.Z());
    }
    if (input->IsMiddleHeld())
    {
        transform->Position = Vec3(transform->Position.X() + input->GetDeltaX() * -xMoveSpeed * deltaTime,
                                   transform->Position.Y() + input->GetDeltaY() * yMoveSpeed * deltaTime,
                                   transform->Position.Z());
    }
}
