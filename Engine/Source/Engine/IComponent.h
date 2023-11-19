#pragma once
#include <memory>

#include "IimGuiDraw.h"

class GameObject;

class IComponent : public IimGuiDraw
{
public:
    
    IComponent(std::weak_ptr<GameObject> object);
    virtual ~IComponent() = default;
    void ImGuiDraw() override {}

    std::weak_ptr<GameObject> gameObject;
};
