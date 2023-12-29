#include "ComponentRegistry.h"

#include "DirectionalLightComponent.h"
#include "Implementation/CameraComponent.h"
#include "Implementation/MeshComponent.h"
#include "implementation/SpriteComponent.h"


void ComponentRegistry::RegisterComponents()
{
    RegisterComponent<MeshComponent>("Mesh");
    RegisterComponent<SpriteComponent>("Sprite");
    RegisterComponent<CameraComponent>("Camera");
    RegisterComponent<DirectionalLightComponent>("Directional Light");
}

template <>
void ComponentRegistry::RegisterComponent<DirectionalLightComponent>(const std::string& alias)
{
    componentCreators[alias] = [](std::any gameObject) -> std::shared_ptr<IComponent>
    {
        auto component = std::make_shared<DirectionalLightComponent>(
            std::any_cast<std::shared_ptr<GameObject>>(gameObject));
        if (const auto& scene = SceneManager::GetScene().lock())
        {
            scene->RegisterDirectionalLight(component);
        }
        return component;
    };
}
