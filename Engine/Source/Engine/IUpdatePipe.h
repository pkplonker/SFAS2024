#pragma once
#include "IUpdateable.h"

class IGraphics;
class IInput;

class IUpdatePipe : public IUpdateable
{
public:
	IUpdatePipe() = default;
	void Update() override = 0;
	virtual void ShutDown() = 0;
	virtual void PreUpdate() = 0;
	virtual void PostUpdate() = 0;

protected:
	IGraphics* Graphics{};
	IInput* Input{};
};
