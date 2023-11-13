#pragma once
#include <DirectXMath.h>

#include "ICamera.h"

class GameObject;

class OrthographicCamera : public ICamera
{
public:
    OrthographicCamera(float width, float height);
    DirectX::XMMATRIX GetProjectionMatrix() override;
    DirectX::XMMATRIX GetViewMatrix() override;
    DirectX::XMMATRIX GetViewProjectionMatrix() override;
    void SetHeight(int height) override;
    void SetWidth(int width) override;

protected:
    DirectX::XMMATRIX view{};
    DirectX::XMMATRIX projection{};
    DirectX::XMMATRIX vpMatrix{};
    float width;
    float height;
};
