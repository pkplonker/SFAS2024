#pragma once
#include <DirectXMath.h>

#include "ICamera.h"

class OrthographicCamera : public ICamera
{
public:
    OrthographicCamera(float width, float height);
    DirectX::XMMATRIX GetProjectionMatrix() override;
    DirectX::XMMATRIX GetViewMatrix() override;
    DirectX::XMVECTOR GetFocusPoint();
    DirectX::XMVECTOR GetEyePosition();
    DirectX::XMVECTOR GetUpDirection();
    DirectX::XMMATRIX GetViewProjectionMatrix() override;
protected:
    DirectX::XMMATRIX view{};
    DirectX::XMMATRIX projection{};
    DirectX::XMMATRIX vpMatrix{};
    bool isDirty;
    float width;
    float height;
};
