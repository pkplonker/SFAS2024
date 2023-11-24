#pragma once
#include "Engine/ICamera.h"

struct Transform3D;
constexpr  float PERS_DEFAULT_NEAR = 0.1f;
constexpr float PERS_DEFAULT_FAR = 200.0f;
constexpr float DEFAULT_FOV = DirectX::XM_PIDIV4;

class PerspectiveCamera : public ICamera
{
public:
	PerspectiveCamera(std::shared_ptr<Transform3D> transform, float width, float height, float near = PERS_DEFAULT_NEAR,
		float far = PERS_DEFAULT_FAR,
		float FOV = DEFAULT_FOV);
	PerspectiveCamera(float width, float height, float near, float far, float FOV);
	DirectX::XMMATRIX GetProjectionMatrix() override;
	DirectX::XMMATRIX GetViewMatrix() override;
	DirectX::XMMATRIX GetViewProjectionMatrix() override;
	void SetTransform(std::shared_ptr<Transform3D> transform) override {this->transform= transform;}

	void SetHeight(float height) override;
	void SetWidth(float width) override;
	void SetNear(float near = PERS_DEFAULT_NEAR);
	void SetFar(float far = PERS_DEFAULT_FAR);
	void SetFOV(float fov = DEFAULT_FOV);
	float GetNear() const;
	float GetFar() const;
	float GetFOV() const;
	float GetWidth() const;
	float GetHeight() const;

	std::shared_ptr<Transform3D> GetTransform() override;

	DirectX::XMVECTOR GetFocusPoint();
	DirectX::XMVECTOR GetEyePosition();
	DirectX::XMVECTOR GetUpDirection();

protected:
	DirectX::XMMATRIX view{};
	DirectX::XMMATRIX projection{};
	DirectX::XMMATRIX vpMatrix{};
	float nearZ;
	float farZ;
	float width;
	float height;
	float FOV;

	std::shared_ptr<Transform3D> transform={};
};
