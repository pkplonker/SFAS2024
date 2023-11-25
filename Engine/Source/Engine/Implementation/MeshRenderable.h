#pragma once

#include "Engine/Implementation/GameObject.h"
#include "Engine/IComponent.h"
#include "Engine/IRenderableComponent.h"
#include "Engine/IRenderable.h"
class MeshRenderable : public IRenderableComponent
{
public:
	MeshRenderable(std::weak_ptr<GameObject> object);

	MeshRenderable(std::weak_ptr<GameObject> object, std::shared_ptr<IRenderable> renderable, std::shared_ptr<IMaterial> material);
};
