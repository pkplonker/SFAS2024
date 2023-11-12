#include "PerspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera(float width, float height, float near, float far): near(near),
    far(far), width(width), height(height), view(DirectX::XMMatrixIdentity()),
    projection(DirectX::XMMatrixIdentity()), vpMatrix(DirectX::XMMatrixIdentity()), isDirty(true)
{
}

DirectX::XMMATRIX PerspectiveCamera::GetProjectionMatrix()
{
    if (!isDirty)return projection;
    const float halfWidth = width / 2;
    const float halfHeight = height / 2;
    projection =
        DirectX::XMMatrixOrthographicOffCenterLH(-halfWidth, halfWidth, -halfHeight, halfHeight, near, far);
    return projection;
}

DirectX::XMMATRIX PerspectiveCamera::GetViewMatrix()
{
    return DirectX::XMMatrixIdentity();
}

DirectX::XMMATRIX PerspectiveCamera::GetViewProjectionMatrix()
{
    if (!isDirty)return vpMatrix;
    GetProjectionMatrix();
    GetViewMatrix();
    vpMatrix = XMMatrixMultiply(view, projection);
    return vpMatrix;
}
