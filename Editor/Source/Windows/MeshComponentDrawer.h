#pragma once
#include "ComponentDrawer.h"

class MeshComponentDrawer : public ComponentDrawer
{
public:
    ~MeshComponentDrawer() override;
    MeshComponentDrawer(std::weak_ptr<IComponent> component);
    void ChangeMesh();
    void Draw() override;
};
