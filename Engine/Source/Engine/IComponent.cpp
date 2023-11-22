#include "IComponent.h"

IComponent::IComponent(std::weak_ptr<GameObject> object) : gameObject(std::move(object))
{
}

std::weak_ptr<GameObject> IComponent::GetGameObject()
{
	return gameObject;
}