#pragma once

#include "Engine/Implementation/GameObject.h"
#include "Engine/IComponent.h"
#include "Engine/IRenderableComponent.h"
#include "Engine/IRenderable.h"
class MeshComponent : public IRenderableComponent
{
public:
	MeshComponent(std::weak_ptr<GameObject> object);
	MeshComponent(std::weak_ptr<GameObject> object, std::shared_ptr<IRenderable> renderable, std::shared_ptr<IMaterial> material);
	std::string GetPath(){return path;}
private:
	std::string path;
};
