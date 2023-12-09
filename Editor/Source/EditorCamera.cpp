#include "EditorCamera.h"

#include <iostream>

#include "IInput.h"
#include "PerspectiveCamera.h"
#include "SceneManager.h"
#include "Transform3D.h"
#include "Implementation/Scene.h"
#include "Logging/Debug.h"

EditorCamera::EditorCamera(IInput* input)
{
    this->input = input;
    transform = std::make_shared<Transform3D>();
    transform->Position = Vec3(0, 0, -12);
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
    auto xSpeed = 1.0f;
    auto ySpeed = 1.0f;
    auto deltaTime = 0.16f;

    if (input->IsRightHeld())
    {
        transform->Rotation = Vec3(transform->Rotation.X() /*+ input->GetDeltaY() * xSpeed * deltaTime*/,
                                   transform->Rotation.Y() + input->GetDeltaX() * ySpeed * deltaTime,
                                   transform->Rotation.Z());
        std::cout << "Setting rot " << input->GetDeltaX() << ":" << input->GetDeltaY() << std::endl;
    }
}
