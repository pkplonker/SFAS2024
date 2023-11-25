#include "SpriteRenderable.h"

SpriteRenderable::SpriteRenderable(std::weak_ptr<GameObject> object) : IRenderableComponent(object)
{
}

SpriteRenderable::SpriteRenderable(std::weak_ptr<GameObject> object, std::shared_ptr<IRenderable> renderable, IShader* shader) : IRenderableComponent(object, renderable,shader)
{
}