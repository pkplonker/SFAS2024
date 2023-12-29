#pragma once
#include <DirectXMath.h>
#include "sstream"
#include "Implementation/XInput/DirectXInput.h"

namespace dx = DirectX;

struct Vec2
{
    Vec2()
    {
        vec = dx::XMFLOAT2(0);
    }
    
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

    Vec2 operator-(const Vec2& other) const
    {
        return Vec2(this->X()-other.X(), this->Y()-other.Y());
    }
    Vec2 operator+(const Vec2& other) const
    {
        return Vec2(this->X()+other.X(), this->Y()+other.Y());
    }

    friend std::ostream& operator<<(std::ostream& os, const Vec2& obj)
    {
        os << "Vec2(x: " << obj.vec.x << ", y: " << obj.vec.y << ")";
        return os;
    }

private:
    dx::XMFLOAT2 vec{};
};
