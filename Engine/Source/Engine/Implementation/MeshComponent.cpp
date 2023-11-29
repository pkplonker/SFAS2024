#include "MeshComponent.h"

#include "IApplication.h"

MeshComponent::MeshComponent(std::weak_ptr<GameObject> object) : IRenderableComponent(object)
{
}

MeshComponent::MeshComponent(std::weak_ptr<GameObject> object, std::shared_ptr<IMeshRenderable> renderable,
                             IMaterial* material) : IRenderableComponent(object, renderable, material)
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

void MeshComponent::SetMesh(Mesh* mesh)
{
    IApplication::GetGraphics()->RemoveRenderable(renderable);
    renderable = IApplication::GetGraphics()->CreateMeshRenderable(this->material, mesh);
    UpdateRenderableTransform();
}

void MeshComponent::SetMaterial(IMaterial* material)
{
    this->material = material;
    IApplication::GetGraphics()->UpdateRenderable(material, renderable);
    UpdateRenderableTransform();
}

void MeshComponent::UpdateRenderableTransform()
{
    if (auto obj = gameObject.lock())
    {
        this->renderable->SetTransform(obj->Transform());
    }
}
