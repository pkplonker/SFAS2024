#pragma once
#include <DirectXMath.h>

#include "Vector2.h"

namespace dx = DirectX;

struct Vec3
{
    Vec3(float x, float y, float z)
    {
        vec = dx::XMFLOAT3(x, y, z);
    }

    Vec3(float x, float y) : Vec3(x, y, 0)
    {
    }

    Vec3(Vec2 val) : Vec3(val.X(), val.Y(), 0)
    {
    }

    static Vec3 Zero()
    {
        return {0,0,0};
    }

    static Vec3 One()
    {
        return {1,1,1};
    }


    [[nodiscard]] float X() const
    {
        return vec.x;
    }

    [[nodiscard]] float Y() const
    {
        return vec.y;
    }

    [[nodiscard]] float Z() const
    {
        return vec.z;
    }

    void X(float val)
    {
        vec.x = val;
    }

    void Y(float val)
    {
        vec.y = val;
    }

    void Z(float val)
    {
        vec.z = val;
    }
    
    void X(int val)
    {
        vec.x = static_cast<float>(val);
    }

    void Y(int val)
    {
        vec.y = static_cast<float>(val);
    }

    void Z(int val)
    {
        vec.z = static_cast<float>(val);
    }

private:
    dx::XMFLOAT3 vec{};
};
