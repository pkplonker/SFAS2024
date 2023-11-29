#pragma once
#include <string>

#include "ImGuiWindow.h"

const std::string RENDER_STAT = "Fps Counter";

class RenderStatWindow : public EditorWindow
{
public:
    void Draw() override;
    std::string GetName() override;
};
