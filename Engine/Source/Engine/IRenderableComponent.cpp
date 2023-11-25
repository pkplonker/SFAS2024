#include "IRenderableComponent.h"

#include "IRenderable.h"

IRenderableComponent::IRenderableComponent(std::weak_ptr<GameObject> object) : IComponent(object)
{
}


IRenderableComponent::IRenderableComponent(std::weak_ptr<GameObject> object, std::shared_ptr<::IRenderable> renderable,
                                           std::shared_ptr<IMaterial> material) : IComponent(object)
{
    this->material = material;
    SetRenderable(renderable);
}

void IRenderableComponent::SetRenderable(std::shared_ptr<::IRenderable> renderable)
{
    this->renderable = renderable;

    if (std::shared_ptr<GameObject> go = gameObject.lock())
    {
        this->renderable->SetTransform(go->Transform());
    }
}
