#include "PerspectiveCamera.h"

#include "Engine/Implementation/Transform3D.h"

PerspectiveCamera::PerspectiveCamera(std::shared_ptr<Transform3D> transform, float width, float height, float near,
	float far, float FOV) : transform(transform), nearZ(near),
	farZ(far), width(width), height(height), FOV(FOV),
	view(DirectX::XMMatrixIdentity()),
	projection(DirectX::XMMatrixIdentity()),
	vpMatrix(DirectX::XMMatrixIdentity())
{
}
PerspectiveCamera::PerspectiveCamera( float width, float height, float near,
	float far, float FOV) : nearZ(near),
	farZ(far), width(width), height(height), FOV(FOV),
	view(DirectX::XMMatrixIdentity()),
	projection(DirectX::XMMatrixIdentity()),
	vpMatrix(DirectX::XMMatrixIdentity())
{
}

DirectX::XMMATRIX PerspectiveCamera::GetProjectionMatrix()
{
	projection = DirectX::XMMatrixPerspectiveFovLH(FOV, width / height, nearZ, farZ);
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
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(
		transform->Rotation.X(), transform->Rotation.Y(), transform->Rotation.Z());
	DirectX::XMVECTOR forwardBase = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	DirectX::XMVECTOR forwardDirection = DirectX::XMVector3TransformNormal(forwardBase, rotationMatrix);
	return DirectX::XMVectorAdd(GetEyePosition(), forwardDirection);
}

DirectX::XMVECTOR PerspectiveCamera::GetEyePosition()
{
	return DirectX::XMVectorSet(transform->Position.X(), transform->Position.Y(), transform->Position.Z(), 1.0f);;
}

DirectX::XMVECTOR PerspectiveCamera::GetUpDirection()
{
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(
		transform->Rotation.X(), transform->Rotation.Y(), transform->Rotation.Z());

	DirectX::XMVECTOR upBase = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	return DirectX::XMVector3TransformNormal(upBase, rotationMatrix);
}

void PerspectiveCamera::SetHeight(float height)
{
	this->height = height;
}

void PerspectiveCamera::SetWidth(float width)
{
	this->width = width;
}

void PerspectiveCamera::SetNear(float near)
{
	this->nearZ = near;
}

void PerspectiveCamera::SetFar(float far)
{
	this->farZ = far;
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