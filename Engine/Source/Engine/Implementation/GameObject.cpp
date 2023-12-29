#include "GameObject.h"

#include <DirectXCollision.h>
#include <iostream>
#include <utility>

#include "Helpers.h"
#include "Engine/IComponent.h"
#include "Transform3D.h"
#include "DirectXMath.h"
#include "IRenderableComponent.h"

GameObject::GameObject()
{
    components = std::make_unique<std::vector<std::shared_ptr<IComponent>>>();
    transform = std::make_shared<Transform3D>();
    this->Name = GAMEOBJECT_DEFAULT_NAME;
    GenerateGUID();
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

DirectX::BoundingBox GameObject::GetAABB()
{
    DirectX::BoundingBox result;
    result.Extents = DirectX::XMFLOAT3(0, 0, 0);

    result.Center = Transform()->Position.vec;

    if (auto renderableComponent = GetComponent<IRenderableComponent>(); renderableComponent != nullptr)
    {
        if (auto renderable = renderableComponent->GetRenderable())
        {
            if (renderable->AllowInteraction())
            {
                Vec3 ext = Transform()->Scale * renderable->GetBounds().Extents;
                result.Extents = ext;
            }
        }
    }
    return result;
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

std::string GameObject::GetGUID() const
{
    return guid;
}

void GameObject::SetGUID(std::string value)
{
    guid = value;
}

std::string GameObject::GenerateGUID()
{
    GUID newGuid;
    CoCreateGuid(&newGuid);
    if (Helpers::TryGetStringFromGuid(newGuid, guid))
    {
        return guid;
    }
    return "";
}
