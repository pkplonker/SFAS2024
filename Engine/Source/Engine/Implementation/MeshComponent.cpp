#include "MeshComponent.h"

MeshComponent::MeshComponent(std::weak_ptr<GameObject> object) : IRenderableComponent(object)
{
}
MeshComponent::MeshComponent(std::weak_ptr<GameObject> object, std::shared_ptr<IMeshRenderable> renderable, IMaterial* material) : IRenderableComponent(object, renderable, material)
{
}

std::string MeshComponent::GetMeshPath()
{
	if (renderable)
	{
		std::shared_ptr<IMeshRenderable> meshRenderable = std::dynamic_pointer_cast<IMeshRenderable>(renderable);
		if (meshRenderable)
		{
			return meshRenderable->GetPath();
		}
	}
	return "";
}

void MeshComponent::SetMesh(std::shared_ptr<IMeshRenderable> mesh)
{
	this->renderable = mesh;
}
