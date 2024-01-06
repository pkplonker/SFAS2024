#pragma once
#include <memory>

#include "CameraComponentDrawer.h"
#include "DirectionalLightComponent.h"
#include "DirectionalLightComponentDrawer.h"
#include "MeshComponentDrawer.h"
#include "PointLightComponentDrawer.h"
#include "SpotLightComponent.h"
#include "SpotLightComponentDrawer.h"
#include "PointLightComponent.h"
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
            if (std::shared_ptr<SpotLightComponent> comp = std::dynamic_pointer_cast<SpotLightComponent>(sharedComponent))
            {
                return std::make_unique<SpotLightComponentDrawer>(comp);
            }
            if (std::shared_ptr<PointLightComponent> comp = std::dynamic_pointer_cast<PointLightComponent>(sharedComponent))
            {
                return std::make_unique<PointLightComponentDrawer>(comp);
            }
        }

        return nullptr;
    }
};
