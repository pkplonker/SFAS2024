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
    auto buttonSize = ImVec2(45, 20);
    ImGui::Begin("ObjectControl", nullptr, ImGuiWindowFlags_NoDecoration);

    if (ImGuiHelpers::ButtonWithState("Translate", ImVec2(80, 20), gizmoController->GetGizmoOperation() == Translation,
                                      true, false))
    {
        gizmoController->SetGizmoOpertion(Translation);
    }
    ImGui::SameLine();
    if (ImGuiHelpers::ButtonWithState("Rotate", ImVec2(60, 20), gizmoController->GetGizmoOperation() == Rotation,
                                      true, false))
    {
        gizmoController->SetGizmoOpertion(Rotation);
    }

    ImGui::SameLine();
    ImGui::Spacing();
    ImGui::SameLine();
    ImGui::Spacing();
    ImGui::SameLine();

    if (ImGuiHelpers::ButtonWithState("Scale", buttonSize, gizmoController->GetGizmoOperation() == Scale, true, false))
    {
        gizmoController->SetGizmoOpertion(Scale);
    }
    ImGui::SameLine();
    if (ImGuiHelpers::ButtonWithState("World", buttonSize, gizmoController->GetMode() == World, true, false))
    {
        gizmoController->SetMode(World);
    }
    ImGui::SameLine();
    if (ImGuiHelpers::ButtonWithState("Local", buttonSize, gizmoController->GetMode() == Local, true, false))
    {
        gizmoController->SetMode(Local);
    }
    ImGui::End();
}

std::string ObjectControlWindow::GetName()
{
    return NAME;
}
