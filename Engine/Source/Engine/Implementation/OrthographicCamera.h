#pragma once
#include <DirectXMath.h>

#include "Engine/ICamera.h"

class GameObject;
const float ORHTO_DEFAULT_NEAR = 0.1f;
const float ORHTO_DEFAULT_FAR = 2000.0f;
class OrthographicCamera : public ICamera
{
public:
	OrthographicCamera(std::shared_ptr<Transform3D> transform, float width, float height, float nearZ = ORHTO_DEFAULT_NEAR, float farZ = ORHTO_DEFAULT_FAR);
	DirectX::XMMATRIX GetProjectionMatrix() override;
	DirectX::XMMATRIX GetViewMatrix() override;
	DirectX::XMMATRIX GetViewProjectionMatrix() override;
	void SetHeight(float height) override;
	void SetWidth(float width) override;
	std::shared_ptr<Transform3D> GetTransform() override;
	void SetNear(float newNear= ORHTO_DEFAULT_NEAR);
	void SetFar(float newFar = ORHTO_DEFAULT_FAR);
	float GetNear(){return nearZ;}
	float GetFar(){return farZ;}

protected:
	DirectX::XMMATRIX view{};
	DirectX::XMMATRIX projection{};
	DirectX::XMMATRIX vpMatrix{};
	float width;
	float height;
	float nearZ;
	float farZ;

	std::shared_ptr<Transform3D> transform;
};


