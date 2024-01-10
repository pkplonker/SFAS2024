#pragma once

class ComponentDrawer
{
public:
	ComponentDrawer();
	virtual ~ComponentDrawer();
	virtual void Draw() = 0;
protected:
	
};
