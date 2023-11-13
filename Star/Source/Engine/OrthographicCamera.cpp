#include "OrthographicCamera.h"

OrthographicCamera::OrthographicCamera(float width, float height) : width(width), height(height)
{
}

DirectX::XMMATRIX OrthographicCamera::GetProjectionMatrix()
{
    float fovAngleY = DirectX::XM_PIDIV4;
    float aspectRatio = width / height;
    float nearZ = 0.1f;
    float farZ = 2000.0f;
    projection = DirectX::XMMatrixPerspectiveFovLH(
        fovAngleY,
        aspectRatio,
        nearZ,
        farZ
    );
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
