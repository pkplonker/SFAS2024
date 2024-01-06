#pragma once
#include "GameObject.h"
#include "IComponent.h"
#include "ILight.h"

class SpotLightComponent : public ILight, public IComponent
{
public:
    SpotLightComponent(std::weak_ptr<GameObject> gameObject);

    Vec3 GetPosition() const;
    void SetPosition(const Vec3& value);

    Vec3 GetDirection() const;


    float GetInnerCone() const;
    void SetInnerCone(float value);

    float GetOuterCone() const;
    void SetOuterCone(float value);

protected:
    float innerCone = 1.0f;
    float outerCone = 1.0f;
};
