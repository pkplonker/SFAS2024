#pragma once

#include "Engine/Implementation/GameObject.h"
#include "Engine/IComponent.h"
#include "Engine/IRenderableComponent.h"
#include "Engine/IRenderable.h"
class SpriteComponent : public IRenderableComponent
{
public:
	SpriteComponent(std::weak_ptr<GameObject> object);
	SpriteComponent(std::weak_ptr<GameObject> object, std::shared_ptr<IRenderable> renderable, IMaterial* material);
};
