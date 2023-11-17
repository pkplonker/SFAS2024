#pragma once
#include "IImGuiRenderable.h"
#include "ImGuiController.h"
const std::string FPS_COUNTER = "FPS_COUNTER";
class ImGuiFPSCounter : public IImGuiRenderable
{
public:
    ImGuiFPSCounter(ImGuiController* ImGui)
    {
        ImGui->RegisterWindow(this, FPS_COUNTER);
    }

    void Render() override;
};


