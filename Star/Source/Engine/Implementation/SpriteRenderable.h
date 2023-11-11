#pragma once
#include <stdexcept>

#include "Engine/GameObject.h"
#include "Engine/IComponent.h"
#include "Engine/IRenderable.h"

class SpriteRenderable : public IComponent
{
public:
    SpriteRenderable(std::weak_ptr<GameObject> object) : IComponent(object)
    {
    }

    SpriteRenderable(std::weak_ptr<GameObject> object, std::shared_ptr<IRenderable> renderable) : IComponent(object)
    {
        SetRenderable(renderable);
    }

    void SetRenderable(std::shared_ptr<IRenderable> renderable)
    {
        this->IRenderable = renderable;
        if (std::shared_ptr<GameObject> go = gameObject.lock())
        {
            this->IRenderable->SetTransform(go->Transform());
        }
    }

private:
    std::shared_ptr<IRenderable> IRenderable = nullptr;
};
