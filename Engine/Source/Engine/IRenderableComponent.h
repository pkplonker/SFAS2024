#pragma once
#include "IComponent.h"
#include "IRenderable.h"

class IRenderableComponent : public IComponent
{
public:
	IRenderableComponent(std::weak_ptr<GameObject> object);

	IRenderableComponent(std::weak_ptr<GameObject> object, std::shared_ptr<IRenderable> renderable, std::shared_ptr<IMaterial> material);

	void SetRenderable(std::shared_ptr<IRenderable> renderable);

	virtual std::shared_ptr<IRenderable> GetRenderable()
	{
		return renderable;
	}

	virtual std::shared_ptr<IMaterial> GetMaterial()
	{
		return material;
	}

protected:
	std::shared_ptr<IRenderable> renderable = nullptr;
	std::shared_ptr<IMaterial> material = material;

};
