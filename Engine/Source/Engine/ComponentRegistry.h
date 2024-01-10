#pragma once
#include <any>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "DirectionalLightComponent.h"
#include "SceneManager.h"
#include "Implementation/Scene.h"
#include "implementation/Logging/Debug.h"


class SpotLightComponent;
class GameObject;
class IComponent;

class ComponentRegistry
{
public:
    template <typename T>
    static void RegisterComponent(const std::string& alias)
    {
        componentCreators[alias] = [](std::any gameObject) -> std::shared_ptr<IComponent>
        {
            return std::make_shared<T>(std::any_cast<std::shared_ptr<GameObject>>(gameObject));
        };
    }

    template <>
    static void RegisterComponent<DirectionalLightComponent>(const std::string& alias);
    template <>
    static void RegisterComponent<SpotLightComponent>(const std::string& alias);
    template <>
    static void RegisterComponent<PointLightComponent>(const std::string& alias);

    static std::shared_ptr<IComponent> CreateComponent(const std::string& alias, std::shared_ptr<GameObject> gameObject)
    {
        auto it = componentCreators.find(alias);
        if (it != componentCreators.end())
        {
            return it->second(gameObject);
        }
        return nullptr;
    }

    static std::vector<std::string> GetComponentTypes()
    {
        std::vector<std::string> keys;
        keys.reserve(componentCreators.size());
        for (const auto& pair : componentCreators)
        {
            keys.push_back(pair.first);
        }
        return keys;
    }

    static void RegisterComponents();

private:
    static inline std::unordered_map<std::string, std::function<std::shared_ptr<IComponent>(std::any)>>
    componentCreators;
};
