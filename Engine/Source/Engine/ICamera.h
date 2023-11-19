#pragma once
#include <DirectXMath.h>

#include "IGraphics.h"

struct Transform3D;

class ICamera
{
public:
    virtual ~ICamera() = default;
    virtual std::shared_ptr<Transform3D> GetTransform() =0;

    ICamera() = default;
    virtual DirectX::XMMATRIX GetProjectionMatrix() = 0;
    virtual DirectX::XMMATRIX GetViewMatrix() = 0;
    virtual DirectX::XMMATRIX GetViewProjectionMatrix() = 0;
    virtual void SetHeight(int getHeight) =0;
    virtual void SetWidth(int getHeight) =0;
};
