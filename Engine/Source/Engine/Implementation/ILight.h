#pragma once
#include "../Math/Vector3.h"

class ILight
{
public:
    ILight(){}

    Vec3 GetColor() const { return color; }
    void SetColor(Vec3 value) { color = value; }
    float GetIntensity() const { return intensity; }
    void SetIntensity(float value) { intensity = value; }

protected:
    Vec3 color = {1, 1, 1};
    float intensity = 1;
};
