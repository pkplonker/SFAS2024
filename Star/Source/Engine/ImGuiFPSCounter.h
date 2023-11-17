#pragma once
#include "IImGuiRenderable.h"
#include "ImGuiController.h"
const std::string FPS_COUNTER = "Fps Counter";
class ImGuiFPSCounter : public IImGuiRenderable
{
public:
    ImGuiFPSCounter(ImGuiController* ImGui)
    {
        ImGui->RegisterWindow(this, FPS_COUNTER);
    }

    void Render(std::string window) override;
};


