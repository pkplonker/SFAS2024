#pragma once
#include "Math/Vector3.h"


struct Transform3D
{
    Transform3D() : Transform3D(Vec3::Zero(), Vec3::Zero(), Vec3::One())
    {
    }

    Transform3D(Vec3 position, Vec3 rotation, Vec3 scale)
    {
        this->Position = position;
        this->Rotation = rotation;
        this->Scale = scale;
    }

    Transform3D(Vec3 position, Vec3 rotation) : Transform3D(position, rotation, Vec3::One())
    {
    }

    Transform3D(Vec3 position) : Transform3D(position, Vec3::Zero(), Vec3::One())
    {
    }

    Vec3 Position = {0};
    Vec3 Rotation = {0};
    Vec3 Scale = {1};
};
