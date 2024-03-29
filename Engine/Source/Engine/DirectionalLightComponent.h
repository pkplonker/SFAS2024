﻿#pragma once
#include "IComponent.h"
#include "Implementation/ILight.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"

class DirectionalLightComponent final : public ILight, public IComponent, std::enable_shared_from_this<DirectionalLightComponent>
{
public:
    DirectionalLightComponent(std::weak_ptr<GameObject> object);
    Vec4 GetDirection();
    ILight::Light& GetLight() override;
    float intensity;

private:
    Vec4 color;
};
