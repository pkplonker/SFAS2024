#include "SpriteComponent.h"

#include "IApplication.h"
#include "ResourceManager.h"

SpriteComponent::SpriteComponent(std::weak_ptr<GameObject> object) : IRenderableComponent(object)
{
    this->SetRenderable(IApplication::GetGraphics()->CreateBillboard(nullptr));
}

SpriteComponent::SpriteComponent(std::weak_ptr<GameObject> object, std::shared_ptr<IRenderable> renderable, IMaterial* material) : IRenderableComponent(object, renderable, material)
{
}
