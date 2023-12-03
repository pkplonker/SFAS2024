#pragma once
#include "IComponent.h"
#include "IRenderable.h"

class IRenderableComponent : public IComponent
{
public:
	IRenderableComponent(std::weak_ptr<GameObject> object);

	IRenderableComponent(std::weak_ptr<GameObject> object, std::shared_ptr<IRenderable> renderable, IMaterial* material);

	void SetRenderable(std::shared_ptr<IRenderable> renderable);

	virtual std::shared_ptr<IRenderable> GetRenderable()
	{
		return renderable;
	}

	virtual IMaterial* GetMaterial()
	{
		return material;
	}

protected:
	std::shared_ptr<IRenderable> renderable = nullptr;
	IMaterial* material = nullptr;
};
