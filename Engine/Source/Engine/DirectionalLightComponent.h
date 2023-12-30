#pragma once
#include "IComponent.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"

class DirectionalLightComponent final : public IComponent, std::enable_shared_from_this<DirectionalLightComponent>
{
public:
    DirectionalLightComponent(std::weak_ptr<GameObject> object);
    Vec4 GetDirection();
    Vec4 GetColor();
    void SetColor(Vec4 col);
    float intensity;

private:
    Vec4 color;
};
