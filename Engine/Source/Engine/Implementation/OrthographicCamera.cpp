#include "OrthographicCamera.h"

#include <memory>

#include "DirectX11/DirectX11Billboard.h"

OrthographicCamera::OrthographicCamera(std::shared_ptr<Transform3D> transform, float width, float height, float nearZ,
                                       float farZ) : transform(transform), width(width),
                                                     height(height), nearZ(nearZ), farZ(farZ)
{
}
OrthographicCamera::OrthographicCamera(float width, float height, float nearZ,
                                       float farZ) : width(width),
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

void OrthographicCamera::SetHeight(float height)
{
    this->height = height;
}

void OrthographicCamera::SetWidth(float width)
{
    this->width = width;
}

std::shared_ptr<Transform3D> OrthographicCamera::GetTransform()
{
    return transform;
}

void OrthographicCamera::SetNear(float newNear)
{
    nearZ = newNear;
}

void OrthographicCamera::SetFar(float newFar)
{
    farZ = newFar;
}
