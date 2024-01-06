#include "PointLightComponent.h"

PointLightComponent::PointLightComponent(std::weak_ptr<GameObject> gameObject): IComponent(gameObject)
{
}

Vec3 PointLightComponent::GetPosition() const
{
    return gameObject.lock()->Transform()->Position;
}

void PointLightComponent::SetPosition(const Vec3& value)
{
    gameObject.lock()->Transform()->Position = value;
}
