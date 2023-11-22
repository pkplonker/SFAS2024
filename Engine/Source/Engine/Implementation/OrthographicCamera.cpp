#include "OrthographicCamera.h"

#include <memory>

#include "DirectX11/DirectX11Billboard.h"

OrthographicCamera::OrthographicCamera(std::shared_ptr<Transform3D> transform, float width, float height, float nearZ,
	float farZ) : transform(transform), width(width),
	height(height), nearZ(nearZ), farZ(farZ)
{
}

DirectX::XMMATRIX OrthographicCamera::GetProjectionMatrix()
{
	projection = DirectX::XMMatrixOrthographicLH(width, height, nearZ, farZ);
	return projection;
}

DirectX::XMMATRIX OrthographicCamera::GetViewMatrix()
{
	view = DirectX::XMMatrixIdentity();
	return view;
}

DirectX::XMMATRIX OrthographicCamera::GetViewProjectionMatrix()
{
	vpMatrix = XMMatrixMultiply(GetViewMatrix(), GetProjectionMatrix());
	return vpMatrix;
}

void OrthographicCamera::SetHeight(int height)
{
	this->height = static_cast<float>(height);
}

void OrthographicCamera::SetWidth(int width)
{
	this->width = static_cast<float>(width);
}

std::shared_ptr<Transform3D> OrthographicCamera::GetTransform()
{
	return transform;
}