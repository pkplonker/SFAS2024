#pragma once
#include "ComponentDrawer.h"

class DirectionalLightComponent;

class DirectionalLightComponentDrawer  : public ComponentDrawer
{
public:
    DirectionalLightComponentDrawer(std::weak_ptr<DirectionalLightComponent> component);

    void Draw() override;
private:
    std::weak_ptr<DirectionalLightComponent> component;
};
