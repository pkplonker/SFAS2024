#include "ObjectControlWindow.h"

#include <iostream>

#include "imgui.h"
#include "../GizmoController.h"
#include "../ImGuiHelpers.h"

ObjectControlWindow::ObjectControlWindow(std::shared_ptr<GizmoController> gizmoController) : gizmoController(std::move(
    gizmoController))
{
}

void ObjectControlWindow::Draw()
{
    auto buttonSize = ImVec2(40, 20);
    auto flags = ImGuiWindowFlags_NoDecoration;
    ImGui::Begin("ObjectControl", nullptr, flags);

    if (ImGuiHelpers::ButtonWithState("Translation", buttonSize, gizmoController->GetGizmoOperation() == Translation))
    {
        gizmoController->SetGizmoOpertion(Translation);
    }
    ImGui::SameLine();
    if (ImGuiHelpers::ButtonWithState("Rotation", buttonSize, gizmoController->GetGizmoOperation() == Rotation))
    {
        gizmoController->SetGizmoOpertion(Rotation);
    }
    ImGui::SameLine();
    if (ImGuiHelpers::ButtonWithState("Scale", buttonSize, gizmoController->GetGizmoOperation() == Scale))
    {
        gizmoController->SetGizmoOpertion(Scale);
    }
    ImGui::SameLine();
    if (ImGuiHelpers::ButtonWithState("World", buttonSize, gizmoController->GetMode() == World))
    {
        gizmoController->SetMode(World);
    }
    ImGui::SameLine();
    if (ImGuiHelpers::ButtonWithState("Local", buttonSize, gizmoController->GetMode() == Local))
    {
        gizmoController->SetMode(Local);
    }
    ImGui::End();
}

std::string ObjectControlWindow::GetName()
{
    return NAME;
}
