#pragma once
#include "IComponent.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"

class DirectionalLightComponent final : public IComponent
{
public:
    DirectionalLightComponent(std::weak_ptr<GameObject> object);
    Vec4 GetDirection();
    Vec4 GetColor();
    void SetColor(Vec4 col);
private:
    Vec4 color;
};
