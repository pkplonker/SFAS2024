#pragma once

#include "IMeshRenderable.h"
#include "Engine/Implementation/GameObject.h"
#include "Engine/IComponent.h"
#include "Engine/IRenderableComponent.h"
#include "Engine/IRenderable.h"

class IMeshRenderable;

class MeshComponent : public IRenderableComponent
{
public:
    MeshComponent(std::weak_ptr<GameObject> object);
    MeshComponent(std::weak_ptr<GameObject> object, std::shared_ptr<IMeshRenderable> renderable,
                  std::shared_ptr<IMaterial> material);

    std::string GetMeshPath();

    std::shared_ptr<IMaterial> GetMaterial() { return material; }

private:
};
