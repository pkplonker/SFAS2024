#include "PointLightComponent.h"

#include "Scene.h"
#include "SceneManager.h"

PointLightComponent::PointLightComponent(std::weak_ptr<GameObject> gameObject): IComponent(gameObject)
{
    light.LightType = PointLight;
}

PointLightComponent::~PointLightComponent()
{
    if (auto scene = SceneManager::GetScene().lock())
    {
        scene->DeregisterLight(shared_from_this());
    }
}

Vec3 PointLightComponent::GetPosition() const
{
    return gameObject.lock()->Transform()->Position;
}

void PointLightComponent::SetPosition(const Vec3& value)
{
    gameObject.lock()->Transform()->Position = value;
}

ILight::Light& PointLightComponent::GetLight()
{
    auto vec = gameObject.lock()->Transform()->Position;
    light.Position = DirectX::XMFLOAT4(vec.X(), vec.Y(), vec.Z(), 1);
    vec = gameObject.lock()->Transform()->GetDirection();
    light.Direction = DirectX::XMFLOAT4(vec.X(), vec.Y(), vec.Z(), 1);
    light.Enabled = gameObject.lock()->GetIsEnabled();
    return light;
}
