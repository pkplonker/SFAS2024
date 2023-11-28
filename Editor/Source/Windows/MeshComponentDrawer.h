#pragma once
#include "ComponentDrawer.h"
#include "MeshComponent.h"

class MeshComponentDrawer : public ComponentDrawer
{
public:
    ~MeshComponentDrawer() override;
    MeshComponentDrawer(std::weak_ptr<MeshComponent> component);
    void ChangeMesh();
    void Draw() override;
private:
    std::weak_ptr<MeshComponent> component;

};
