#include "DirectionalLightComponent.h"

#include "Implementation/GameObject.h"

DirectionalLightComponent::DirectionalLightComponent(const std::weak_ptr<GameObject> object) : IComponent(object), color(Vec4(1,1,1,1))
{
}

Vec4 DirectionalLightComponent::GetDirection()
{
    Vec4 result ={};
    if(const auto& go = gameObject.lock())
    {
       result= Vec4( go->Transform()->Rotation,1);
    }
    return result;
}

Vec4 DirectionalLightComponent::GetColor()
{
    return color;
}

void DirectionalLightComponent::SetColor(Vec4 col)
{
    this->color=col;
}
