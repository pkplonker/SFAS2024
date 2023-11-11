#include "IRenderable.h"

const Transform2D& IRenderable::GetTransform() const
{
    return Transform;
}

void IRenderable::SetPosition(float x, float y)
{
    Transform.Position.X = x;
    Transform.Position.Y = y;
}

void IRenderable::SetPosition(Vector2 position)
{
    Transform.Position = position;
}

void IRenderable::SetRotation(float r)
{
    Transform.Rotation = r;
}

void IRenderable::SetScale(float sx, float sy)
{
    Transform.Scale.X = sx;
    Transform.Scale.Y = sy;
}

void IRenderable::SetScale(Vector2 scale)
{
    Transform.Scale = scale;
}
