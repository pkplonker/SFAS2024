#pragma once
#include "ComponentDrawer.h"
#include "MaterialDrawerHelper.h"
#include "MeshComponent.h"

class MeshComponentDrawer : public ComponentDrawer
{
public:
    ~MeshComponentDrawer() override;
    MeshComponentDrawer(std::weak_ptr<MeshComponent> component);
    void Draw() override;
    void ChangeMesh();

private:
    std::weak_ptr<MeshComponent> component;
    MaterialDrawerHelper materialDrawerHelper;
};
