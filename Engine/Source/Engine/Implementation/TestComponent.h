#pragma once
#include "Engine/IComponent.h"
#include "Engine/IUpdateable.h"

class TestComponent : public IComponent, public IUpdateable
{
public:
    TestComponent(std::weak_ptr<GameObject> object) : IComponent(object)
    {
    }

    void Update() override;
};
