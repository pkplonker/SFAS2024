#pragma once
#include <DirectXMath.h>

namespace dx = DirectX;

struct Vec2
{
    Vec2(float x, float y)
    {
        vec = dx::XMFLOAT2(x, y);
    }

    Vec2(float val) : Vec2(val, val)
    {
    }

    static Vec2 Zero()
    {
        return {0};
    }

    static Vec2 One()
    {
        return {1};
    }

    [[nodiscard]] float X() const
    {
        return vec.x;
    }

    [[nodiscard]] float Y() const
    {
        return vec.y;
    }

    void X(float val)
    {
        vec.x = val;
    }

    void Y(float val)
    {
        vec.y = val;
    }

    void X(int val)
    {
        vec.x = static_cast<float>(val);
    }

    void Y(int val)
    {
        vec.y = static_cast<float>(val);
    }
    bool operator==(const Vec2& vec2) const
    {
        return X() == vec2.X() && Y() == vec2.Y();
    }

private:
    dx::XMFLOAT2 vec{};
};
