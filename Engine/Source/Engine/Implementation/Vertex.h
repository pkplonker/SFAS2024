#pragma once
#include "Engine/Math/Vector2.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Vector4.h"

struct Vertex
{
    Vec3 position;
    Vec4 color;
    Vec3 normal;
    Vec2 texture;

    Vertex(const Vec3 position) : position(position), texture(Vec2(0,0)), normal(Vec3(1,0,0)), color(Vec4(1,0,1,1)) {};
    Vertex(const Vec3 position, const  Vec3 &normal) : position(position), color(Vec4(1, 0, 1, 1)), normal(normal), texture(Vec2(0, 0)) {};
    Vertex(const Vec3 position, const  Vec4 color) : position(position), color(color), normal(Vec3(1, 0, 0)), texture(Vec2(0, 0)) {};
    Vertex(const Vec3 position, const Vec4 color, const  Vec3& normal) : position(position), color(color), normal(normal), texture(Vec2(0, 0)) {};
    Vertex(const Vec3 position, const Vec2 texture, const Vec3& normal, const Vec4 &color) : position(position), color(color), normal(normal), texture(texture) {};
    bool operator == (const Vertex& vert);
    bool Compare(const Vertex& vertex);
};
