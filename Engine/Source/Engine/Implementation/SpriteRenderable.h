#pragma once

#include "Engine/Implementation/GameObject.h"
#include "Engine/IComponent.h"
#include "Engine/IRenderableComponent.h"
#include "Engine/IRenderable.h"
class SpriteRenderable : public IRenderableComponent
{
public:
	SpriteRenderable(std::weak_ptr<GameObject> object);
	SpriteRenderable(std::weak_ptr<GameObject> object, std::shared_ptr<IRenderable> renderable);
};
