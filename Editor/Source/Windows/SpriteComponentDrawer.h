#pragma once
#include "ComponentDrawer.h"
#include "MaterialDrawerHelper.h"

class SpriteComponent;

class SpriteComponentDrawer : public ComponentDrawer
{
public:
    ~SpriteComponentDrawer() override;
    SpriteComponentDrawer(std::weak_ptr<SpriteComponent> component);
    void Draw() override;
private:
    std::weak_ptr<SpriteComponent> component;
    MaterialDrawerHelper materialDrawerHelper;
};
