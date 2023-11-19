#include "IRenderable.h"

std::weak_ptr<Transform3D> IRenderable::GetTransform()
{
	return Transform;
}