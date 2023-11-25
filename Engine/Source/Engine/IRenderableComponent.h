#pragma once
#include "IComponent.h"
#include "IRenderable.h"

class IRenderableComponent : public IComponent
{
public:
	IRenderableComponent(std::weak_ptr<GameObject> object);

	IRenderableComponent(std::weak_ptr<GameObject> object, std::shared_ptr<IRenderable> renderable, IShader* shader);

	void SetRenderable(std::shared_ptr<IRenderable> renderable);

	virtual std::shared_ptr<IRenderable> GetRenderable()
	{
		return renderable;
	}

	virtual IShader* GetShader()
	{
		return shader;
	}

protected:
	std::shared_ptr<IRenderable> renderable = nullptr;
	IShader* shader = shader;

};
