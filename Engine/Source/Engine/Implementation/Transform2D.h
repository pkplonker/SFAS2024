#pragma once
#include "Engine/Math/Vector2.h"

struct Transform2D
{
	Transform2D() : Transform2D(Vec2::Zero(), 0, Vec2::One())
	{
	}

	Transform2D(Vec2 position, float rotation, Vec2 scale)
	{
		this->Position = position;
		this->Rotation = rotation;
		this->Scale = scale;
	}

	Transform2D(Vec2 position, float rotation) : Transform2D(position, rotation, Vec2::One())
	{
	}

	Transform2D(Vec2 position) : Transform2D(position, 0, Vec2::One())
	{
	}

	Transform2D(Vec2 position, Vec2 scale) : Transform2D(position, 0, scale)
	{
	}

	Vec2 Position = { 0 };
	float Rotation = 0;
	Vec2 Scale = { 1 };
};
