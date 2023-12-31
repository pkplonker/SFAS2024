#pragma once
#include <DirectXCollision.h>

#include "EditorWindow.h"
const std::string DEBUG_CONTROLS = "Debug Controls";

class DebugControls : public EditorWindow
{
public:
    DebugControls();
    ~DebugControls() override;
    void Draw() override;
    std::string GetName() override;

private:
    void DrawBounds();
    bool drawBounds = false;
    bool drawGrid = false;
    std::string drawBoundsKey ="DEBUG_DRAW_BOUNDS";
    std::string drawGridKey ="DEBUG_DRAW_GRID";

};
