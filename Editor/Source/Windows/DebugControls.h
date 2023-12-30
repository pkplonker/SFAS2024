#pragma once
#include <DirectXCollision.h>

#include "EditorWindow.h"
const std::string DEBUG_CONTROLS = "Debug Controls";

class DebugControls : public EditorWindow
{
public:
    void Draw() override;
    std::string GetName() override;
};
