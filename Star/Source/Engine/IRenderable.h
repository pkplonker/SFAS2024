#pragma once

#include "Engine/IGraphics.h"
#include "Engine/Transform2D.h"
#include "IUpdateable.h"

class IRenderable : public IUpdateable
{
public:
    virtual void Update() = 0;

    Transform2D& GetTransform();

private:
    Transform2D Transform;
};
