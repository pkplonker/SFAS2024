#pragma once
#include "ComponentDrawer.h"

class CameraComponentDrawer : public ComponentDrawer
{
public:
	virtual ~CameraComponentDrawer();
	CameraComponentDrawer(std::weak_ptr<IComponent> component);
	void Draw() override;
};
