#include "ObjectControlWindow.h"

#include <iostream>

#include "imgui.h"

void ObjectControlWindow::Draw()
{
    auto buttonSize = ImVec2(50, 30);
    auto flags = ImGuiWindowFlags_NoDecoration;
    ImGui::Begin("ObjectControl", nullptr, flags);

    if (ImGui::Button("Test", buttonSize))
    {
        std::cout << "testing" << std::endl;
    }
    ImGui::End();
}

std::string ObjectControlWindow::GetName()
{
    return NAME;
}
