#pragma once
#include <memory>

#include "CameraComponentDrawer.h"
#include "DirectionalLightComponent.h"
#include "DirectionalLightComponentDrawer.h"
#include "MeshComponentDrawer.h"
#include "SpriteComponent.h"
#include "SpriteComponentDrawer.h"
#include "Engine/Implementation/CameraComponent.h"
#include "Engine/Implementation/MeshComponent.h"

class DirectionalLightComponent;
class ComponentDrawer;

class ComponentDrawerFactory
{
public:
    std::unique_ptr<ComponentDrawer> CreateDrawer(std::weak_ptr<IComponent> component)
    {
        if (auto sharedComponent = component.lock())
        {
            if (std::shared_ptr<CameraComponent> comp = std::dynamic_pointer_cast<CameraComponent>(sharedComponent))
            {
                return std::make_unique<CameraComponentDrawer>(comp);
            }
            if (std::shared_ptr<MeshComponent> comp = std::dynamic_pointer_cast<MeshComponent>(sharedComponent))
            {
                return std::make_unique<MeshComponentDrawer>(comp);
            }
            if (std::shared_ptr<SpriteComponent> comp = std::dynamic_pointer_cast<SpriteComponent>(sharedComponent))
            {
                return std::make_unique<SpriteComponentDrawer>(comp);
            }
            if (std::shared_ptr<DirectionalLightComponent> comp = std::dynamic_pointer_cast<DirectionalLightComponent>(sharedComponent))
            {
                return std::make_unique<DirectionalLightComponentDrawer>(comp);
            }
        }

        return nullptr;
    }
};
