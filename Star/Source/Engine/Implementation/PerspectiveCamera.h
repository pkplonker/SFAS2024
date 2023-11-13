#pragma once
#include "Engine/ICamera.h"

const float DEFAULT_NEAR = 0.1f;
const float DEFAULT_FAR = 1000.0f;


class PerspectiveCamera : public ICamera
{
public:
    PerspectiveCamera(float width, float height, float near = DEFAULT_NEAR, float far = DEFAULT_FAR);
    DirectX::XMMATRIX GetProjectionMatrix() override;
    DirectX::XMMATRIX GetViewMatrix() override;
    DirectX::XMMATRIX GetViewProjectionMatrix() override;
    void SetHeight(int height) override;
    void SetWidth(int width) override;
    DirectX::XMVECTOR GetFocusPoint();
    DirectX::XMVECTOR GetEyePosition();
    DirectX::XMVECTOR GetUpDirection();
protected:
    DirectX::XMMATRIX view{};
    DirectX::XMMATRIX projection{};
    DirectX::XMMATRIX vpMatrix{};
    float near;
    float far;
    float width;
    float height;
};
