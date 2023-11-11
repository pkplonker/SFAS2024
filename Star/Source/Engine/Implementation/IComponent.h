#pragma once
#include <memory>

class GameObject;

class IComponent 
{
public:
    IComponent(std::weak_ptr<GameObject> object);
    std::weak_ptr<GameObject> gameObject;
};
