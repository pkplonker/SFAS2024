#include "CameraComponent.h"

CameraComponent::CameraComponent(std::weak_ptr<GameObject> object): IComponent(object), camera(nullptr)
{
}

std::shared_ptr<ICamera> CameraComponent::GetCamera()
{
    return camera;
}

DirectX::XMMATRIX CameraComponent::GetViewMatrix()
{
    return camera->GetViewMatrix();
}

CameraComponent::CameraComponent(std::weak_ptr<GameObject> object, std::shared_ptr<ICamera> camera): IComponent(object),
    camera(camera)
{
}

void CameraComponent::SetCamera(std::shared_ptr<ICamera> cam)
{
    camera = cam;
}

DirectX::XMMATRIX CameraComponent::GetProjectionMatrix()
{
    return camera->GetProjectionMatrix();
}

DirectX::XMMATRIX CameraComponent::GetViewProjectionMatrix()
{
    return camera->GetViewProjectionMatrix();
}

void CameraComponent::SetHeight(int height)
{
    camera->SetHeight(static_cast<float>(height));
}

void CameraComponent::SetWidth(int width)
{
    camera->SetWidth(static_cast<float>(width));;
}
