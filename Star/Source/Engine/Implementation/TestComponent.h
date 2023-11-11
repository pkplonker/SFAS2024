#pragma once
#include "IComponent.h"
#include "IUpdateable.h"

class TestComponent : public IComponent, public IUpdateable
{
public:
    TestComponent(std::shared_ptr<GameObject> object) : IComponent(object){}
    void Update() override;
};
