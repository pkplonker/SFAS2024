#pragma once
#include "ComponentDrawer.h"
#include "SpotLightComponent.h"


class SpotLightComponentDrawer : public ComponentDrawer {
public:
    SpotLightComponentDrawer(std::weak_ptr<SpotLightComponent> component)
        : ComponentDrawer(), component(component) {}

    void Draw() override;

private:
    std::weak_ptr<SpotLightComponent> component;
};