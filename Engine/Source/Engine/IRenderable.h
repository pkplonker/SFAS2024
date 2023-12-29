#pragma once

#include "Implementation/GameObject.h"
#include "Engine/IGraphics.h"
#include "Implementation/Transform3D.h"

class IRenderable
{
public:
	virtual ~IRenderable() = default;
	virtual void Update() = 0;
	virtual std::weak_ptr<Transform3D> GetTransform();
	virtual unsigned int GetVertsCount() =0;
	virtual unsigned int GetTriangleCount() = 0;
	
	void SetTransform(std::shared_ptr<Transform3D> transform)
	{
		this->Transform = transform;
	}

private:
	std::shared_ptr<Transform3D> Transform;
};
