#pragma once

#include "Engine/IMeshRenderable.h"
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
        IMaterial* material);

    std::string GetMeshPath();

    IMaterial* GetMaterial() { return material; }

private:
};
