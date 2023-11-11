#include "GameObject.h"

#include "IComponent.h"
#include "Engine/Debug.h"

GameObject::GameObject()
{
    components = std::make_unique<std::vector<std::shared_ptr<IComponent>>>();
}

void GameObject::Update()
{
    for (const auto& component : *components)
    {
        auto updateable = std::dynamic_pointer_cast<IUpdateable>(component);

        if (updateable != nullptr)
        {
            updateable->Update();
        }
    }
    Debug("Updated GO")
}


