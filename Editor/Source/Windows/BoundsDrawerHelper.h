#pragma once
#include "../../Packages/directxtk_desktop_2019.2023.9.6.1/include/DirectXHelpers.h"

namespace DirectX
{
    struct BoundingBox;
}

class BoundsDrawerHelper
{
public:
    static void DrawBounds(DirectX::BoundingBox& aabb);
};
