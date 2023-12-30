#pragma once
#include <DirectXMath.h>
#include <iostream>
#include <sstream>

#include "Vector2.h"


namespace dx = DirectX;

struct Vec3
{
    Vec3()
    {
        vec.x = 0;
        vec.y = 0;
        vec.z = 0;
    }

    Vec3(float x, float y, float z)
    {
        vec = dx::XMFLOAT3(x, y, z);
    }

    Vec3(float x, float y) : Vec3(x, y, 0)
    {
    }

    Vec3(float x) : Vec3(x, x, x)
    {
    }

    Vec3(Vec2 val) : Vec3(val.X(), val.Y(), 0)
    {
    }

    Vec3(float* data)
    {
        if (data != nullptr)
        {
            X(data[0]);
            Y(data[1]);
            Z(data[2]);
        }
    }

    Vec3(DirectX::XMVECTOR xmvector)
    {
        DirectX::XMStoreFloat3(&vec, xmvector);
    }

    Vec3(DirectX::XMFLOAT3 val)
    {
        vec = val;
    }

    static Vec3 Zero()
    {
        return {0, 0, 0};
    }

    static Vec3 One()
    {
        return {1, 1, 1};
    }

    static Vec3 Up()
    {
        return {0, 1, 0};
    }

    static Vec3 Down()
    {
        return {0, -1, 0};
    }

    static Vec3 Left()
    {
        return {-1, 0, 0};
    }

    static Vec3 Right()
    {
        return {1, 0, 0};
    }

    static Vec3 Forward()
    {
        return {0, 0, 1};
    }

    static Vec3 Back()
    {
        return {0, 0, -1};
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

    float* Data()
    {
        return reinterpret_cast<float*>(&vec);
    }

    bool operator==(const Vec3& vec3) const
    {
        return vec.x == vec3.vec.x && vec.y == vec3.vec.y && vec.z == vec3.vec.z;
    }

    Vec3 operator+(const Vec3& vec3) const
    {
        return Vec3(vec.x + vec3.vec.x, vec.y + vec3.vec.y, vec.z + vec3.vec.z);
    }

    Vec3 operator-(const Vec3& vec3) const
    {
        return Vec3(vec.x - vec3.vec.x, vec.y - vec3.vec.y, vec.z - vec3.vec.z);
    }

    Vec3 operator*(float scalar) const
    {
        return Vec3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
    }

    Vec3 operator*(int scalar) const
    {
        return Vec3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
    }

    Vec3 operator*(const DirectX::XMMATRIX& m)
    {
        DirectX::XMVECTOR vec = DirectX::XMVectorSet(X(), Y(), Z(), 1.0f);
        DirectX::XMVECTOR resultVec = DirectX::XMVector3Transform(vec, m);
        return Vec3(DirectX::XMVectorGetX(resultVec), DirectX::XMVectorGetY(resultVec),
                    DirectX::XMVectorGetZ(resultVec));
    }

    friend std::ostream& operator<<(std::ostream& os, const Vec3& obj)
    {
        os << "Vec3(x: " << obj.vec.x << ", y: " << obj.vec.y << ", Z: " << obj.vec.z << ")";
        return os;
    }

    Vec3&& operator*(const DirectX::XMFLOAT3& other) const
    {
        return Vec3(vec.x * other.x, vec.y * other.y, vec.z * other.z);
    }

    operator DirectX::XMFLOAT3() const
    {
        return vec;
    }

    dx::XMFLOAT3 vec{};
};
