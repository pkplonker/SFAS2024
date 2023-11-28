#pragma once
#include <memory>

#include "CameraComponentDrawer.h"
#include "MeshComponentDrawer.h"
#include "Engine/Implementation/CameraComponent.h"
#include "Engine/Implementation/MeshComponent.h"

class ComponentDrawer;

class ComponentDrawerFactory
{
public:
	std::unique_ptr<ComponentDrawer> CreateDrawer(std::weak_ptr<IComponent> component)
	{
		if (auto sharedComponent = component.lock())
		{
			if (dynamic_cast<CameraComponent*>(sharedComponent.get())) {
				return std::make_unique<CameraComponentDrawer>(sharedComponent);
			}
			if (dynamic_cast<MeshComponent*>(sharedComponent.get())) {
				return std::make_unique<MeshComponentDrawer>(sharedComponent);
			}
		}

		return nullptr;
	}
};
