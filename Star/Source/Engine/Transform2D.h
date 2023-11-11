
#pragma once
#include "Implementation/Math/Vector2.h"

struct Transform2D
{
	Vector2 Position = Vector2::Zero();
	float Rotation = 0;
	Vector2 Scale = Vector2::One();
};

