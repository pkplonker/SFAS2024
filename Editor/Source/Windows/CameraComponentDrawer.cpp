#include "CameraComponentDrawer.h"

#include <utility>

#include "imgui.h"
#include "Engine/Implementation/CameraComponent.h"
#include "Engine/Implementation/GameObject.h"
#include "Engine/Implementation/OrthographicCamera.h"
#include "Engine/Implementation/PerspectiveCamera.h"
CameraComponentDrawer::~CameraComponentDrawer() = default;

CameraComponentDrawer::CameraComponentDrawer(std::weak_ptr<IComponent> component) : ComponentDrawer(
    std::move(component))
{
}

void CameraComponentDrawer::DrawPerspective(std::shared_ptr<PerspectiveCamera> cam)
{
    if (ImGui::TreeNodeEx("Perspective Camera Settings", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::BeginPopupContextItem("PerspectiveCameraContext"))
        {
            if (ImGui::MenuItem("Reset Near"))
            {
                cam->SetNear();
            }
            if (ImGui::MenuItem("Reset Far"))
            {
                cam->SetFar();
            }
            if (ImGui::MenuItem("Reset FOV"))
            {
                cam->SetFOV();
            }
            ImGui::EndPopup();
        }
        float fov = cam->GetFOV();
        if (ImGui::SliderFloat("Field of View", &fov, 0.01f, 3.141f))
        {
            cam->SetFOV(fov);
        }

        float newNear = cam->GetNear();
        if (ImGui::DragFloat("Near Plane", &newNear, 1, 0.1f))
        {
            cam->SetNear(newNear);
        }

        float newFar = cam->GetFar();
        if (ImGui::DragFloat("Far Plane", &newFar, 1, 0.1f))
        {
            if (newFar > newNear)
            {
                cam->SetFar(newFar);
            }
        }

        ImGui::TreePop();
    }
}

void CameraComponentDrawer::Draw()
{
    if (std::shared_ptr<IComponent> sharedComponent = component.lock())
    {
        if (auto cameraComponent = std::dynamic_pointer_cast<CameraComponent>(sharedComponent))
        {
            if (ImGui::CollapsingHeader("Camera Component", ImGuiTreeNodeFlags_DefaultOpen))
            {
                if (ImGui::BeginPopupContextItem("CameraComponentContext"))
                {
                    if (ImGui::MenuItem("Delete component"))
                    {
                        if (auto gameobject = cameraComponent->GetGameObject().lock())
                        {
                            gameobject->RemoveComponent(cameraComponent);
                        }
                    }
                    ImGui::EndPopup();
                }

                if (auto camera = cameraComponent->GetCamera())
                {
                    if (auto cam = std::dynamic_pointer_cast<PerspectiveCamera>(camera))
                    {
                        DrawPerspective(cam);
                    }
                    else if (auto cam = std::dynamic_pointer_cast<OrthographicCamera>(camera))
                    {
                        if (ImGui::TreeNodeEx("Orthographic Camera Settings", ImGuiTreeNodeFlags_DefaultOpen))
                        {
                            if (ImGui::BeginPopupContextItem("OrthographicCameraContext"))
                            {
                                if (ImGui::MenuItem("Reset Near"))
                                {
                                    cam->SetNear();
                                }
                                if (ImGui::MenuItem("Reset Far"))
                                {
                                    cam->SetFar();
                                }
                                ImGui::EndPopup();
                            }
                            float newNearZ = cam->GetNear();
                            if (ImGui::DragFloat("Near Plane", &newNearZ))
                            {
                                cam->SetNear(newNearZ);
                            }

                            float newFarZ = cam->GetFar();
                            if (ImGui::DragFloat("Far Plane", &newFarZ))
                            {
                                cam->SetFar(newFarZ);
                            }

                            ImGui::TreePop();
                        }
                    }
                    else
                    {
                        ImGui::Text("No camera attached");
                    }
                }
            }
        }
    }
}
