#include "GameObject.h"
#include <utility>
#include "IComponent.h"
#include "Engine/Debug.h"
#include "Engine/Transform3D.h"

GameObject::GameObject()
{
    components = std::make_unique<std::vector<std::shared_ptr<IComponent>>>();
    transform = std::make_unique<Transform3D>();
    this->name = GAMEOBJECT_DEFAULT_NAME;
}

GameObject::GameObject(std::unique_ptr<Transform3D> transform) : GameObject()
{
    if (transform)
    {
        this->transform = std::move(transform);
    }
}

GameObject::GameObject(std::string name, std::unique_ptr<Transform3D> transform) : GameObject()
{
    if (transform)
    {
        this->transform = std::move(transform);
    }
    this->name = std::move(name);
}

GameObject::GameObject(std::string name) : GameObject()
{
    this->name = std::move(name);
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
    //Debug("Updated GO")
}
