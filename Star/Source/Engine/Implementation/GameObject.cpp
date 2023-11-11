#include "GameObject.h"

GameObject::GameObject()
{
    components = std::make_unique<std::vector<std::shared_ptr<IComponent>>>();
}

bool GameObject::AddComponent(const std::shared_ptr<IComponent>& component)
{
    if (component != nullptr)
    {
        components->emplace_back(component);
        return true;
    }
    return false;
}

bool GameObject::RemoveComponent(const std::shared_ptr<IComponent>& component)
{
    const auto index = std::find(components->begin(), components->end(), component );
    if(index != components->end())
    {
        components->erase(index);
        return true;
    }
    return false;
}

void GameObject::Update()
{
    for (const auto& component : *components)
    {
        auto updateable = std::reinterpret_pointer_cast<IUpdateable>(component);
        if(updateable != nullptr)
        {
            updateable->Update();
        }
    }
}

