﻿#include "GameObject.h"
#include <utility>
#include "Engine/IComponent.h"
#include "Engine/Implementation/Logging/Debug.h"
#include "Transform3D.h"

GameObject::GameObject()
{
    components = std::make_unique<std::vector<std::shared_ptr<IComponent>>>();
    transform = std::make_shared<Transform3D>();
    this->Name = GAMEOBJECT_DEFAULT_NAME;
    CoCreateGuid(&guid);
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
    this->Name = std::move(name);
}

GameObject::GameObject(std::string name) : GameObject()
{
    this->Name = std::move(name);
}

void GameObject::Init()
{
    transform->SetGameObject(shared_from_this());
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

const std::vector<std::shared_ptr<IComponent>>& GameObject::GetComponents() const
{
    return *components;
}

void GameObject::SetTransform(const std::shared_ptr<Transform3D> transform)
{
    this->transform = transform;
    transform->SetGameObject(shared_from_this());
}

GUID GameObject::GetGUID() const
{
    return guid;
}

void GameObject::SetGUID(GUID value)
{
    guid = value;
}

GUID GameObject::GenerateGUID()
{
    CoCreateGuid(&guid);
    return guid;
}
