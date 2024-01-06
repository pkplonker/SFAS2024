#pragma once
#include "../Math/Vector3.h"

class ILight
{
public:
    virtual ~ILight() = default;

    ILight()
    {
    }

    virtual Vec3 GetColor() const { return color; }
    virtual void SetColor(Vec3 value) { color = value; }
    virtual float GetIntensity() const { return intensity; }
    virtual void SetIntensity(float value) { intensity = value; }

protected:
    Vec3 color = {1, 1, 1};
    float intensity = 1;
};
