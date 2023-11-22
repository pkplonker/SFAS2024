#pragma once
#include "ComponentDrawer.h"
#include "Engine/Implementation/PerspectiveCamera.h"

class CameraComponentDrawer : public ComponentDrawer
{
public:
	virtual ~CameraComponentDrawer();
	CameraComponentDrawer(std::weak_ptr<IComponent> component);
	void DrawPerspective(std::shared_ptr<PerspectiveCamera> cam);
	void Draw() override;
};
