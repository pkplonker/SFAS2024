#pragma once
#include "Engine/IComponent.h"

class ComponentDrawer
{
public:
	ComponentDrawer(std::weak_ptr<IComponent> component);
	virtual ~ComponentDrawer();
	virtual void Draw() = 0;
protected:
	std::weak_ptr<IComponent> component;
};
