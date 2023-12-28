#include "ObjectControlWindow.h"

#include <iostream>

#include "imgui.h"
#include "../GizmoController.h"

ObjectControlWindow::ObjectControlWindow(std::shared_ptr<GizmoController> gizmoController) : gizmoController(std::move(
    gizmoController))
{
}

void ObjectControlWindow::Draw()
{
    auto buttonSize = ImVec2(40, 20);
    auto flags = ImGuiWindowFlags_NoDecoration;
    ImGui::Begin("ObjectControl", nullptr, flags);

    if (ImGui::Button("Translation", buttonSize))
    {
        gizmoController->SetGizmoOpertion(Translation);
    }
    ImGui::SameLine();
    if (ImGui::Button("Rotation", buttonSize))
    {
        gizmoController->SetGizmoOpertion(Rotation);
    }
    ImGui::SameLine();
    if (ImGui::Button("Scale", buttonSize))
    {
        gizmoController->SetGizmoOpertion(Scale);
    }
    ImGui::SameLine();
    if (ImGui::Button("World", buttonSize))
    {
        gizmoController->SetMode(World);
    }
    ImGui::SameLine();
    if (ImGui::Button("Local", buttonSize))
    {
        gizmoController->SetMode(Local);
    }
    ImGui::End();
}

std::string ObjectControlWindow::GetName()
{
    return NAME;
}
