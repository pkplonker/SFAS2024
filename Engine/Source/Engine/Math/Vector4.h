#pragma once
#include "Math/Vector3.h"
#include "Math/Vector2.h"

struct Vec4
{
    Vec4()
    {
        vec = dx::XMFLOAT4(0,0,0,0);
    }

    Vec4(float x, float y, float z, float w)
    {
        vec = dx::XMFLOAT4(x, y, z, w);
    }

    Vec4(float x, float y, float z) : Vec4(x, y, z, 0)
    {
    }

    Vec4(float x, float y) : Vec4(x, y, 0, 0)
    {
    }

    Vec4(float x) : Vec4(x, x, x, x)
    {
    }

    Vec4(Vec2 val) : Vec4(val.X(), val.Y(), 0, 0)
    {
    }

    Vec4(Vec2 val, int z, int w) : Vec4(val.X(), val.Y(), static_cast<float>(z), static_cast<float>(w))
    {
    }

    Vec4(Vec2 val, float z, float w) : Vec4(val.X(), val.Y(), z, w)
    {
    }

    Vec4(Vec3 val) : Vec4(val.X(), val.Y(), val.Z(), 0)
    {
    }

    Vec4(Vec3 val, float w) : Vec4(val.X(), val.Y(), val.Z(), w)
    {
    }

    Vec4(Vec3 val, int w) : Vec4(val.X(), val.Y(), val.Z(), static_cast<float>(w))
    {
    }

    static Vec4 Zero()
    {
        return {0, 0, 0, 0};
    }

    static Vec4 One()
    {
        return {1, 1, 1, 1};
    }

    [[nodiscard]] float& X()
    {
        return vec.x;
    }

    [[nodiscard]] float& Y()
    {
        return vec.y;
    }

    [[nodiscard]] float& Z()
    {
        return vec.z;
    }

    [[nodiscard]] float& W()
    {
        return vec.w;
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

    void W(float val)
    {
        vec.w = val;
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

    void W(int val)
    {
        vec.w = static_cast<float>(val);
    }

    float* Data()
    {
        return reinterpret_cast<float*>(&vec);
    }

    bool operator==(const Vec4& vec4) const
    {
        return vec.x == vec4.vec.x && vec.y == vec4.vec.y && vec.z == vec4.vec.z && vec.w == vec4.vec.w;
    }

    Vec4 Normalise()
    {
        auto length = Length();
        if (length == 0) return Vec4(0, 0, 0, 0);

        return Vec4(vec.x / length, vec.y / length, vec.z / length, vec.w / length);
    }

    float Length()
    {
        return std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w);
    }

private:
    dx::XMFLOAT4 vec{};
};
