#pragma once
#include <DirectXMath.h>

class ICamera
{
public:
    virtual ~ICamera() = default;
    ICamera() = default;
    virtual DirectX::XMMATRIX GetProjectionMatrix() = 0;
    virtual DirectX::XMMATRIX GetViewMatrix() = 0;
    virtual DirectX::XMMATRIX GetViewProjectionMatrix() = 0;
};
