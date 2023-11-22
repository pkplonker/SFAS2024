#pragma once
#include <memory>

#include "CameraComponentDrawer.h"
#include "Engine/Implementation/CameraComponent.h"

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
		}

		return nullptr;
	}
};
