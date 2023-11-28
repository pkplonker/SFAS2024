#pragma once
#include "ComponentDrawer.h"

class SpriteComponentDrawer : public ComponentDrawer
{
public:
    ~SpriteComponentDrawer() override;
    SpriteComponentDrawer(std::weak_ptr<IComponent> component);
    void Draw() override;
};
