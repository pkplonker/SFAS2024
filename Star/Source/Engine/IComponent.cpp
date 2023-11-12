#include "IComponent.h"

IComponent::IComponent(std::weak_ptr<GameObject> object) : gameObject(std::move(object))
{
    
}
