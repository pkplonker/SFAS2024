#include "DebugControls.h"

#include "imgui.h"
#include "SceneManager.h"
#include "scene.h"
#include "../DebugDrawer.h"
#include "Implementation/Scene.h"
#include "../EditorSettings.h"

void DebugControls::DrawBounds()
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

DebugControls::DebugControls()
{
    drawBounds = EditorSettings::Get(drawBoundsKey, true);
    drawGrid = EditorSettings::Get(drawGridKey, true);
}

DebugControls::~DebugControls()
{
    EditorSettings::Set(drawBoundsKey, drawBounds);
    EditorSettings::Set(drawGridKey, drawGrid);
}

void DebugControls::Draw()
{
    ImGui::Begin(DEBUG_CONTROLS.c_str());

    if (ImGui::Checkbox("Draw Bounds", &drawBounds))
    {
    }
    ImGui::SameLine();
    if (ImGui::Checkbox("Draw Grid", &drawGrid))
    {
    }

    DebugDrawer::DrawGrid(drawGrid);

    if (drawBounds)
    {
        DrawBounds();
    }


    ImGui::End();
}

std::string DebugControls::GetName()
{
    return DEBUG_CONTROLS;
}
