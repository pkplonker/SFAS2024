#pragma once
#include <memory>
#include <vector>

#include "IUpdateable.h"

class IComponent;

class GameObject : IUpdateable
{
public:
    GameObject();
    ~GameObject() override {}

    bool AddComponent(const std::shared_ptr<IComponent>& component);
    bool RemoveComponent(const std::shared_ptr<IComponent>& component);

    template <typename T>
    std::shared_ptr<T> GetComponent() const
    {
        for (auto& comp : components)
        {
            std::shared_ptr<T> castedComp = std::dynamic_pointer_cast<T>(comp);
            if (castedComp)
            {
                return castedComp;
            }
        }
        return nullptr;
    }

    void Update() override;

private:
    std::unique_ptr<std::vector<std::shared_ptr<IComponent>>> components;
};
