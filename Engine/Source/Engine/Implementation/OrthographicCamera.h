#pragma once
#include <DirectXMath.h>

#include "Engine/ICamera.h"
#include "Engine/IimGuiDraw.h"

class GameObject;

class OrthographicCamera : public ICamera, public IimGuiDraw
{
public:
	OrthographicCamera(std::shared_ptr<Transform3D> transform, float width, float height, float nearZ = DEFAULT_NEAR, float farZ = DEFAULT_FAR);
	DirectX::XMMATRIX GetProjectionMatrix() override;
	DirectX::XMMATRIX GetViewMatrix() override;
	DirectX::XMMATRIX GetViewProjectionMatrix() override;
	void SetHeight(int height) override;
	void SetWidth(int width) override;
	void ImGuiDraw() override;
	std::shared_ptr<Transform3D> GetTransform() override;

protected:
	DirectX::XMMATRIX view{};
	DirectX::XMMATRIX projection{};
	DirectX::XMMATRIX vpMatrix{};
	float width;
	float height;
	float nearZ;
	float farZ;
	static inline constexpr float DEFAULT_NEAR = 0.1f;
	static inline constexpr float DEFAULT_FAR = 2000.0f;
	std::shared_ptr<Transform3D> transform;
};
