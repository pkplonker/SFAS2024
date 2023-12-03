#pragma once
#include "Engine/IComponent.h"

class ComponentDrawer
{
public:
	ComponentDrawer();
	virtual ~ComponentDrawer();
	virtual void Draw() = 0;
protected:
};
