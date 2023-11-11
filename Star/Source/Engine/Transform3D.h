#pragma once
#include "Implementation/Math/Vector3.h"

struct Transform2D
{
    Vector3 Position = Vector3::Zero();
    float Rotation = 0;
    Vector3 Scale = Vector3::One();
};
