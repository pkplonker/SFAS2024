#pragma once
#include <DirectXMath.h>

#include "Engine/ICamera.h"
#include "Engine/IComponent.h"

struct Transform3D;

class CameraComponent : public IComponent, public ICamera, public std::enable_shared_from_this<CameraComponent>

{
public:
	CameraComponent(std::weak_ptr<GameObject> object);
	CameraComponent(std::weak_ptr<GameObject> object, std::shared_ptr<ICamera> camera);
	void SetCamera(std::shared_ptr<ICamera> cam);
	std::shared_ptr<ICamera> GetCamera();
	DirectX::XMMATRIX GetProjectionMatrix() override;
	DirectX::XMMATRIX GetViewMatrix() override;
	DirectX::XMMATRIX GetViewProjectionMatrix() override;
	void SetHeight(float height);
	void SetWidth(float width);
	std::shared_ptr<Transform3D> GetTransform() override;

private:
	std::shared_ptr<ICamera> camera;
};
