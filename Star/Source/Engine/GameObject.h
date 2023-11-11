﻿#pragma once
#include <memory>
#include <string>
#include <vector>

#include "IUpdateable.h"

#include "Transform3D.h"
class IComponent;

class GameObject : IUpdateable
{
    std::string GAMEOBJECT_DEFAULT_NAME;

public:
    GameObject();
    GameObject(std::unique_ptr<Transform3D> transform);
    GameObject(std::string name, std::unique_ptr<Transform3D> transform);
    GameObject(std::string name);

    template <typename T>
    bool AddComponent(std::shared_ptr<T> component)
    {
        static_assert(std::is_base_of<IComponent, T>::value, "T must be a derived class of IComponent");

        if (component != nullptr)
        {
            components->emplace_back(std::static_pointer_cast<IComponent>(component));
            return true;
        }
        return false;
    }

    template <typename T>
    bool RemoveComponent(const std::shared_ptr<T>& component)
    {
        static_assert(std::is_base_of<IComponent, T>::value, "T must be a derived class of IComponent");

        const std::shared_ptr<IComponent> baseComponent = std::static_pointer_cast<IComponent>(component);
        const auto index = std::find(components->begin(), components->end(), baseComponent);
        if (index != components->end())
        {
            components->erase(index);
            return true;
        }
        return false;
    }

    template <typename T>
    std::shared_ptr<T> GetComponent() const
    {
        for (auto& comp : components)
        {
            std::shared_ptr<T> castComp = std::dynamic_pointer_cast<T>(comp);
            if (castComp)
            {
                return castComp;
            }
        }
        return nullptr;
    }

    void Update() override;

private:
    std::unique_ptr<std::vector<std::shared_ptr<IComponent>>> components;
    std::unique_ptr<Transform3D> transform;
    std::string name = GAMEOBJECT_DEFAULT_NAME;
};