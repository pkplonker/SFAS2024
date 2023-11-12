#pragma once
#include "Engine/ICamera.h"

const float DEFAULT_NEAR = 0.1f;
const float DEFAULT_FAR = 10.1f;


class PerspectiveCamera : public ICamera
{
public:
    PerspectiveCamera(float width, float height, float near = DEFAULT_NEAR, float far = DEFAULT_FAR);
    DirectX::XMMATRIX GetProjectionMatrix() override;
    DirectX::XMMATRIX GetViewMatrix() override;
    DirectX::XMMATRIX GetViewProjectionMatrix() override;

protected:
    DirectX::XMMATRIX view{};
    DirectX::XMMATRIX projection{};
    DirectX::XMMATRIX vpMatrix{};
    float near;
    float far;
    float width;
    float height;
    bool isDirty;
};
