#pragma once
#include "IComponent.h"
#include "IRenderable.h"

class IRenderableComponent : public IComponent
{
public:
    IRenderableComponent(std::weak_ptr<GameObject> object);

    IRenderableComponent(std::weak_ptr<GameObject> object, std::shared_ptr<IRenderable> renderable);

    void SetRenderable(std::shared_ptr<IRenderable> renderable);

    virtual std::shared_ptr<IRenderable> GetRenderable()
    {
        return renderable;
    }

protected:
    std::shared_ptr<IRenderable> renderable = nullptr;

};
