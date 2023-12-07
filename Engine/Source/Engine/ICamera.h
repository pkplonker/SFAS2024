#pragma once
#include <DirectXMath.h>

#include "IGraphics.h"

struct Transform3D;

class ICamera
{
public:
    virtual ~ICamera() = default;
    virtual std::shared_ptr<Transform3D> GetTransform() = 0;
    virtual void SetTransform(std::shared_ptr<Transform3D> transform) =0;

    ICamera() = default;
    virtual DirectX::XMMATRIX GetProjectionMatrix() = 0;
    virtual DirectX::XMMATRIX GetViewMatrix() = 0;
    virtual DirectX::XMMATRIX GetViewProjectionMatrix() = 0;
    virtual void SetHeight(float getHeight) = 0;
    virtual void SetWidth(float getHeight) = 0;
    virtual DirectX::XMFLOAT3 GetCameraForward();
};

inline DirectX::XMFLOAT3 ICamera::GetCameraForward()
{
    DirectX::XMMATRIX viewMatrix = GetViewMatrix();
    DirectX::XMFLOAT4X4 viewMatrixValues;
    DirectX::XMStoreFloat4x4(&viewMatrixValues, viewMatrix);

    DirectX::XMFLOAT3 forward(
        -viewMatrixValues._31,
        -viewMatrixValues._32,
        -viewMatrixValues._33
    );

    DirectX::XMVECTOR forwardVec = DirectX::XMLoadFloat3(&forward);
    forwardVec = DirectX::XMVector3Normalize(forwardVec);
    DirectX::XMStoreFloat3(&forward, forwardVec);

    return forward;
}
