#pragma once
#include <DirectXMath.h>

#include "IGraphics.h"

class ICamera
{
public:
    virtual ~ICamera() = default;

    ICamera() = default;
    virtual DirectX::XMMATRIX GetProjectionMatrix() = 0;
    virtual DirectX::XMMATRIX GetViewMatrix() = 0;
    virtual DirectX::XMMATRIX GetViewProjectionMatrix() = 0;
    virtual void SetHeight(int getHeight) =0;
    virtual void SetWidth(int getHeight) =0;
};
