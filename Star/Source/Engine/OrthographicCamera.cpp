#include "OrthographicCamera.h"

OrthographicCamera::OrthographicCamera(float width, float height) : width(width), height(height), isDirty(true)
{
}

DirectX::XMMATRIX OrthographicCamera::GetProjectionMatrix()
{
    if (!isDirty)return projection;
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
    if (!isDirty)return view;
    DirectX::XMVECTOR EyePosition = GetEyePosition();
    DirectX::XMVECTOR FocusPoint = GetFocusPoint();
    DirectX::XMVECTOR UpDirection = GetUpDirection();
    view = DirectX::XMMatrixLookAtLH(EyePosition, FocusPoint, UpDirection);
    return view;
}

DirectX::XMVECTOR OrthographicCamera::GetFocusPoint()
{
    return DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);;
}

DirectX::XMVECTOR OrthographicCamera::GetEyePosition()
{
    return DirectX::XMVectorSet(0.0f, 0.0f, -10.0f, 1.0f);;
}

DirectX::XMVECTOR OrthographicCamera::GetUpDirection()
{
    return DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
}

DirectX::XMMATRIX OrthographicCamera::GetViewProjectionMatrix()
{
    if (!isDirty)return vpMatrix;
    GetProjectionMatrix();
    GetViewMatrix();
    vpMatrix = XMMatrixMultiply(view, projection);
    return vpMatrix;
}
