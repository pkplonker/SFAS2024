#include "DirectionalLightComponent.h"

#include "SceneManager.h"
#include "Implementation/GameObject.h"
#include "Implementation/Scene.h"

DirectionalLightComponent::DirectionalLightComponent(const std::weak_ptr<GameObject> object) : IComponent(object),
    color(Vec4(1, 1, 1, 1)), intensity(1)
{
    light.LightType = DirectionalLight;

}

Vec4 DirectionalLightComponent::GetDirection()
{
    Vec4 result = {};
    if (const auto& go = gameObject.lock())
    {
        result = Vec4(go->Transform()->Rotation, 1);
    }
    return result.Normalise();
}

ILight::Light& DirectionalLightComponent::GetLight()
{
    auto vec = gameObject.lock()->Transform()->Position;
    vec = gameObject.lock()->Transform()->GetDirection();
    light.Direction = DirectX::XMFLOAT4(vec.X(), vec.Y(), vec.Z(), 1);
    light.Enabled = gameObject.lock()->GetIsEnabled();
    return light;
}

