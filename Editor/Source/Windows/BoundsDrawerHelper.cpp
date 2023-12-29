#include "BoundsDrawerHelper.h"

#include <DirectXCollision.h>

#include "../ImGuiHelpers.h"

void BoundsDrawerHelper::DrawBounds(DirectX::BoundingBox& aabb)
{
    auto extents = aabb.Extents;
    auto extentsVec = Vec3(extents);
    ImGuiHelpers::DrawVector("Extents", extentsVec);
    aabb.Extents = extentsVec;
}
