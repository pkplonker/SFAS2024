#include "DebugControls.h"

#include "imgui.h"
#include "SceneManager.h"
#include "scene.h"
#include "../DebugDrawer.h"
#include "Implementation/Scene.h"

void DebugControls::Draw()
{
    ImGui::Begin(DEBUG_CONTROLS.c_str());
    static bool drawBounds = true;
    if (ImGui::Checkbox("Draw Bounds", &drawBounds))
    {
    }
    if (drawBounds)
    {
        if (const auto scene = SceneManager::GetScene().lock())
        {
            for (auto [GUID, gameObject] : scene->GetObjects())
            {
                if (!gameObject->GetIsEnabled())continue;
                const auto& bounds = gameObject->GetAABB();
                DebugDrawer::DrawBoundingBox(bounds);
            }
        }
    }
    ImGui::End();
}

std::string DebugControls::GetName()
{
    return DEBUG_CONTROLS;
}
