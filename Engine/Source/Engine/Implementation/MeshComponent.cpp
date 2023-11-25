#include "MeshComponent.h"

MeshComponent::MeshComponent(std::weak_ptr<GameObject> object) : IRenderableComponent(object)
{
}

MeshComponent::MeshComponent(std::weak_ptr<GameObject> object, std::shared_ptr<IRenderable> renderable, std::shared_ptr<IMaterial> material) : IRenderableComponent(object, renderable,material)
{
}