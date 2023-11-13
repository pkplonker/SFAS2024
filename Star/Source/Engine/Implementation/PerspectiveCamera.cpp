#include "PerspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera(float width, float height, float near, float far): near(near),
    far(far), width(width), height(height), view(DirectX::XMMatrixIdentity()),
    projection(DirectX::XMMatrixIdentity()), vpMatrix(DirectX::XMMatrixIdentity())
{
}

DirectX::XMMATRIX PerspectiveCamera::GetProjectionMatrix()
{
    float fovAngleY = DirectX::XM_PIDIV4;
    float aspectRatio = static_cast<float>(width) / height;
    projection = DirectX::XMMatrixPerspectiveFovLH(
        fovAngleY,
        aspectRatio,
        near,
        far
    );
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
    return DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);;
}

DirectX::XMVECTOR PerspectiveCamera::GetEyePosition()
{
    return DirectX::XMVectorSet(0.0f, 0.0f, -10.0f, 1.0f);;
}

DirectX::XMVECTOR PerspectiveCamera::GetUpDirection()
{
    return DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
}

void PerspectiveCamera::SetHeight(int height)
{
    this->height = static_cast<float>(height);
}

void PerspectiveCamera::SetWidth(int width)
{
    this->width = static_cast<float>(width);
}
