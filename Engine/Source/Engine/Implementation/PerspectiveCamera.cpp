#include "PerspectiveCamera.h"

#include "Engine/Implementation/Transform3D.h"

PerspectiveCamera::PerspectiveCamera(std::shared_ptr<Transform3D> transform, float width, float height, float nearZ,
	float farZ, float FOV) : transform(transform), nearZ(nearZ),
	farZ(farZ), width(width), height(height), FOV(FOV),
	view(DirectX::XMMatrixIdentity()),
	projection(DirectX::XMMatrixIdentity()),
	vpMatrix(DirectX::XMMatrixIdentity())
{
}
PerspectiveCamera::PerspectiveCamera( float width, float height, float nearZ,
	float farZ, float FOV) : nearZ(nearZ),
	farZ(farZ), width(width), height(height), FOV(FOV),
	view(DirectX::XMMatrixIdentity()),
	projection(DirectX::XMMatrixIdentity()),
	vpMatrix(DirectX::XMMatrixIdentity())
{
}

DirectX::XMMATRIX PerspectiveCamera::GetProjectionMatrix()
{
	projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(FOV), width / height, nearZ, farZ);
	return projection;
}

DirectX::XMMATRIX PerspectiveCamera::GetViewMatrix()
{
	view = DirectX::XMMatrixLookAtLH(GetEyePosition(), GetFocusPoint(), GetUpDirection());
	return view;
}

DirectX::XMMATRIX PerspectiveCamera::GetViewProjectionMatrix()
{
	GetProjectionMatrix();
	GetViewMatrix();
	vpMatrix = XMMatrixMultiply(view, projection);
	return vpMatrix;
}

DirectX::XMVECTOR PerspectiveCamera::GetFocusPoint()
{
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw( DirectX::XMConvertToRadians(transform->Rotation.X())
		, DirectX::XMConvertToRadians(transform->Rotation.Y()), 0);
	DirectX::XMVECTOR forwardBase = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	DirectX::XMVECTOR forwardDirection = DirectX::XMVector3TransformCoord(forwardBase, rotationMatrix);
	return DirectX::XMVectorAdd(GetEyePosition(), forwardDirection);
}

DirectX::XMVECTOR PerspectiveCamera::GetEyePosition()
{
	return DirectX::XMVectorSet(transform->Position.X(), transform->Position.Y(), transform->Position.Z(), 1.0f);;
}

DirectX::XMVECTOR PerspectiveCamera::GetUpDirection()
{
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw( DirectX::XMConvertToRadians(transform->Rotation.X())
		, DirectX::XMConvertToRadians(transform->Rotation.Y()), DirectX::XMConvertToRadians(transform->Rotation.Z()));

	DirectX::XMVECTOR upBase = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	return DirectX::XMVector3TransformCoord(upBase, rotationMatrix);
}

void PerspectiveCamera::SetHeight(float height)
{
	this->height = height;
}

void PerspectiveCamera::SetWidth(float width)
{
	this->width = width;
}

void PerspectiveCamera::SetNear(float newNear)
{
	this->nearZ = newNear;
}

void PerspectiveCamera::SetFar(float newFar)
{
	this->farZ = newFar;
}

void PerspectiveCamera::SetFOV(float fov)
{
	this->FOV = fov;
}

float PerspectiveCamera::GetNear() const
{
	return nearZ;
}

float PerspectiveCamera::GetFar() const
{
	return farZ;
}

float PerspectiveCamera::GetFOV() const
{
	return FOV;
}

float PerspectiveCamera::GetWidth() const
{
	return width;
}

float PerspectiveCamera::GetHeight() const
{
	return height;
}

std::shared_ptr<Transform3D> PerspectiveCamera::GetTransform()
{
	return transform;
}