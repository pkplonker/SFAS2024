#pragma once
#include "ComponentDrawer.h"
#include "Engine/Implementation/PerspectiveCamera.h"

class CameraComponent;

class CameraComponentDrawer : public ComponentDrawer
{
public:
	virtual ~CameraComponentDrawer();
	CameraComponentDrawer(std::weak_ptr<CameraComponent> component);
	void DrawPerspective(std::shared_ptr<PerspectiveCamera> cam);
	void Draw() override;
private:
	std::weak_ptr<CameraComponent> component;
};
