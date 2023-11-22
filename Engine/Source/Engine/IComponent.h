#pragma once
#include <memory>

class GameObject;

class IComponent
{
public:

	IComponent(std::weak_ptr<GameObject> object);
	virtual ~IComponent() = default;
	std::weak_ptr<GameObject> GetGameObject();
	std::weak_ptr<GameObject> gameObject;
};
