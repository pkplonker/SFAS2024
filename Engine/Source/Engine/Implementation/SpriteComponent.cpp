#include "SpriteComponent.h"

SpriteComponent::SpriteComponent(std::weak_ptr<GameObject> object) : IRenderableComponent(object)
{
}

SpriteComponent::SpriteComponent(std::weak_ptr<GameObject> object, std::shared_ptr<IRenderable> renderable, IMaterial* material) : IRenderableComponent(object, renderable, material)
{
}