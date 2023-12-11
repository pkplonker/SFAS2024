#include "EditorCameraWindow.h"

#include "CameraComponentDrawer.h"
#include "imgui.h"
#include "Inspector.h"
#include "../EditorCamera.h"

EditorCameraWindow::EditorCameraWindow(std::shared_ptr<EditorCamera> camera) : camera(std::move(camera))
{
}

void EditorCameraWindow::Draw()
{
    ImGui::Begin(CAMERA_WINDOW.c_str());
    Inspector::DrawVector("Position", camera->GetTransform()->Position);
    Inspector::DrawVector("Rotation", camera->GetTransform()->Rotation);
    Inspector::DrawVector("Scale", camera->GetTransform()->Scale);
  

    CameraComponentDrawer::DrawCameraControls(camera->camera);
    ImGui::End();
}

std::string EditorCameraWindow::GetName()
{
    return CAMERA_WINDOW;
}
