#include "CameraComponent.h"

#include "Engine/Implementation/GameObject.h"
#include "Engine/Implementation/Transform3D.h"

CameraComponent::CameraComponent(std::weak_ptr<GameObject> object) : IComponent(object), camera(nullptr)
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

CameraComponent::CameraComponent(std::weak_ptr<GameObject> object, std::shared_ptr<ICamera> camera) : IComponent(object),
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
	camera->SetHeight(height);
}

void CameraComponent::SetWidth(int width)
{
	camera->SetWidth(width);;
}

std::shared_ptr<Transform3D> CameraComponent::GetTransform()
{
	if (auto object = gameObject.lock())
	{
		return object->Transform();
	}
	return nullptr;
}