#include "MeshRenderable.h"

MeshRenderable::MeshRenderable(std::weak_ptr<GameObject> object) : IRenderableComponent(object)
{
}

MeshRenderable::MeshRenderable(std::weak_ptr<GameObject> object, std::shared_ptr<IRenderable> renderable, IShader* shader) : IRenderableComponent(object, renderable,shader)
{
}