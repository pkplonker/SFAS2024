﻿#pragma once
#include "GameObject.h"
#include "../IComponent.h"
#include "ILight.h"

class PointLightComponent : public ILight, public IComponent
{
public:
    PointLightComponent(std::weak_ptr<GameObject> gameObject);

    Vec3 GetPosition() const;
    void SetPosition(const Vec3& value);
};