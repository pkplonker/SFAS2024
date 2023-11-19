#pragma once
#include "Engine/ICamera.h"
#include "Engine/IimGuiDraw.h"


class PerspectiveCamera : public ICamera, public IimGuiDraw
{
public:
    PerspectiveCamera(float width, float height, float near = DEFAULT_NEAR, float far = DEFAULT_FAR,
                      float FOV = DEFAULT_FOV);
    DirectX::XMMATRIX GetProjectionMatrix() override;
    DirectX::XMMATRIX GetViewMatrix() override;
    DirectX::XMMATRIX GetViewProjectionMatrix() override;
    void SetHeight(int height) override;
    void SetWidth(int width) override;
    void SetNear(float near);
    void SetFar(float far);
    void SetFOV(float fov);


    DirectX::XMVECTOR GetFocusPoint();
    DirectX::XMVECTOR GetEyePosition();
    DirectX::XMVECTOR GetUpDirection();
    void ImGuiDraw() override;

protected:
    DirectX::XMMATRIX view{};
    DirectX::XMMATRIX projection{};
    DirectX::XMMATRIX vpMatrix{};
    float near;
    float far;
    float width;
    float height;
    float FOV;
    static inline constexpr float DEFAULT_NEAR = 0.1f;
    static inline constexpr float DEFAULT_FAR = 1000.0f;
    static inline constexpr float DEFAULT_FOV = DirectX::XM_PIDIV4;
};
