#include "EditorCameraWindow.h"

#include "CameraComponentDrawer.h"
#include "imgui.h"
#include "Inspector.h"
#include "../EditorCamera.h"
#include "../ImGuiHelpers.h"

EditorCameraWindow::EditorCameraWindow(std::shared_ptr<EditorCamera> camera) : camera(std::move(camera))
{
}

void EditorCameraWindow::Draw()
{
    ImGui::Begin(CAMERA_WINDOW.c_str());
    ImGuiHelpers::DrawVector("Position", camera->GetTransform()->Position);
    ImGuiHelpers::DrawVector("Rotation", camera->GetTransform()->Rotation);
    ImGuiHelpers::DrawVector("Scale", camera->GetTransform()->Scale);
  

    CameraComponentDrawer::DrawCameraControls(camera->camera);
    ImGui::End();
}

std::string EditorCameraWindow::GetName()
{
    return CAMERA_WINDOW;
}
