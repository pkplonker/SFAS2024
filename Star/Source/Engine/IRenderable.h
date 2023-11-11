
#pragma once

#include "Engine/IGraphics.h"
#include "Engine/Transform2D.h"

class IRenderable
{
public:

	virtual void Update() = 0;

	const Transform2D& GetTransform() const;
	void SetPosition(float x, float y);
	void SetPosition(Vector2 position);
	void SetRotation(float r);
	void SetScale(float sx, float sy);
	void SetScale(Vector2 scale);

private:
	
	Transform2D Transform;

};

