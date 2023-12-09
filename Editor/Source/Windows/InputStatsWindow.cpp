#include "InputStatsWindow.h"


#include "IInput.h"
#include "imgui.h"

InputStatsWindow::InputStatsWindow(IInput* input) : input(input)
{
}

void InputStatsWindow::Draw()
{
    ImGui::Begin("Inputs Stats");
    if (input != nullptr)
    {
        ImGui::Text("Mouse Pos: %d x %d", input->GetMouseX(), input->GetMouseY());
        ImGui::Text("Mouse Delta: %d x %d", input->GetDeltaX(), input->GetDeltaY());
        if(input->GetDeltaX()!=0 || input->GetDeltaY()!=0) count++;
        ImGui::Text("Delta Change Count: %d", count);

    }
    ImGui::End();
}

std::string InputStatsWindow::GetName()
{
    return INPUT_STAT;
}
