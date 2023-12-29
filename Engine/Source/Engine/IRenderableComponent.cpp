#include "IRenderableComponent.h"

#include "IApplication.h"
#include "IRenderable.h"
#include "SceneManager.h"
#include "Implementation/Scene.h"

IRenderableComponent::IRenderableComponent(std::weak_ptr<GameObject> object) : IComponent(object)
{
}


IRenderableComponent::IRenderableComponent(std::weak_ptr<GameObject> object, std::shared_ptr<::IRenderable> renderable,
                                           IMaterial* material) : IComponent(object)
{
    this->material = material;
    if (renderable != nullptr)
    {
        SetRenderable(renderable);
    }
}

IRenderableComponent::~IRenderableComponent()
{
    const auto scene = SceneManager::GetScene();
    if(const auto shared = scene.lock())
    {
        shared->RemoveRenderable(renderable);
    }
}

void IRenderableComponent::SetRenderable(std::shared_ptr<::IRenderable> renderable)
{
    if (renderable == nullptr) return;
    this->renderable = renderable;

    if (std::shared_ptr<GameObject> go = gameObject.lock())
    {
        this->renderable->SetTransform(go->Transform());
    }
}

void IRenderableComponent::UpdateRenderableTransform()
{
    if (renderable == nullptr)return;
    if (auto obj = gameObject.lock())
    {
        this->renderable->SetTransform(obj->Transform());
    }
}

void IRenderableComponent::SetIsEnabled(bool state)
{
    IComponent::SetIsEnabled(state);
    renderable->SetIsEnabled(state);
}

void IRenderableComponent::SetMaterial(IMaterial* material)
{
    if (auto scene = SceneManager::GetScene().lock())
    {
        this->material = material;
        IApplication::GetGraphics()->UpdateRenderable(material, renderable);
        UpdateRenderableTransform();
    }
}
