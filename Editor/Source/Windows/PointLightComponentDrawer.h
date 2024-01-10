#pragma once
#include "ComponentDrawer.h"
#include "PointLightComponent.h"


class PointLightComponentDrawer : public ComponentDrawer
{
public:
    PointLightComponentDrawer(std::weak_ptr<PointLightComponent> component)
        : ComponentDrawer(), component(component)
    {
    }

    void Draw() override;

private:
    std::weak_ptr<PointLightComponent> component;
};
