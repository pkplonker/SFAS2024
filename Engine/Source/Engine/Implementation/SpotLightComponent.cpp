#include "SpotLightComponent.h"

#include "Scene.h"
#include "SceneManager.h"

SpotLightComponent::SpotLightComponent(std::weak_ptr<GameObject> gameObject): IComponent(gameObject)
{
}

SpotLightComponent::~SpotLightComponent()
{
    if (auto scene = SceneManager::GetScene().lock())
    {
        scene->DeregisterLight(shared_from_this());
    }
}

Vec3 SpotLightComponent::GetPosition() const
{
    return gameObject.lock()->Transform()->Position;
}

void SpotLightComponent::SetPosition(const Vec3& value)
{
    gameObject.lock()->Transform()->Position = value;
}

Vec3 SpotLightComponent::GetDirection() const
{
    Vec3 result = {};
    if (const auto& go = gameObject.lock())
    {
        result = go->Transform()->Rotation;
    }
    return result.Normalise();
}

float SpotLightComponent::GetInnerCone() const
{
    return innerCone;
}

void SpotLightComponent::SetInnerCone(float value)
{
    innerCone = value;
}

float SpotLightComponent::GetOuterCone() const
{
    return outerCone;
}

void SpotLightComponent::SetOuterCone(float value)
{
    outerCone = value;
}

ILight::Light& SpotLightComponent::GetLight()
{
    auto pos = gameObject.lock()->Transform()->Position;
    light.Position = DirectX::XMFLOAT4(pos.X(), pos.Y(), pos.Z(), 1);
    light.Enabled = gameObject.lock()->GetIsEnabled();

    return light;
}
