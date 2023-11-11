#pragma once

struct Vector3
{
    Vector3(float x, float y, float z)
    {
        this->X = x;
        this->Y = y;
        this->Z = z;
    }

    Vector3(int x, int y, int z)
    {
        this->X = x;
        this->Y = y;
        this->Z = z;
    }

    float X;
    float Y;
    float Z;

    static Vector3 Zero() noexcept
    {
        return {0.0f, 0.0f, 0.0f};
    }

    static Vector3 One() noexcept
    {
        return {1.0f, 1.0f, 1.0f};
    }
};
