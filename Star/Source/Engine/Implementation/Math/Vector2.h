#pragma once

struct Vector2
{
    Vector2(float x, float y)
    {
        this->X = x;
        this->Y = y;
    }

    Vector2(int x, int y)
    {
        this->X = x;
        this->Y = y;
    }

    float X = 0.0f;
    float Y = 0.0f;

    static Vector2 Zero() noexcept
    {
        return {0.0f, 0.0f};
    }

    static Vector2 One() noexcept
    {
        return {1.0f, 1.0f};
    }
};
