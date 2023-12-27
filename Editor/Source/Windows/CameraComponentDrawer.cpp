#include "CameraComponentDrawer.h"

#include <utility>

#include "imgui.h"
#include "../ImGuiHelpers.h"
#include "../UndoManager.h"
#include "Engine/Implementation/CameraComponent.h"
#include "Engine/Implementation/GameObject.h"
#include "Engine/Implementation/OrthographicCamera.h"
#include "Engine/Implementation/PerspectiveCamera.h"
#include "Logging/Debug.h"
CameraComponentDrawer::~CameraComponentDrawer() = default;

CameraComponentDrawer::CameraComponentDrawer(std::weak_ptr<CameraComponent> component) : component(component)
{
}

void CameraComponentDrawer::DrawPerspective(std::shared_ptr<PerspectiveCamera> cam)
{
    if (ImGui::TreeNodeEx("Perspective Camera Settings", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::BeginPopupContextItem("PerspectiveCameraContext"))
        {
            float current = cam->GetNear();
            if (ImGui::MenuItem("Reset Near"))
            {
                UndoManager::Execute(
                    Memento(
                        [cam] { cam->SetNear(); },
                        [cam, current] { cam->SetNear(current); },
                        "Resetting near"));
            }


            if (ImGui::MenuItem("Reset Far"))
            {
                auto current = cam->GetFar();
                UndoManager::Execute(
                    Memento(
                        [cam] { cam->SetFar(); },
                        [cam, current] { cam->SetFar(current); },
                        "Resetting far"));
            }

            if (ImGui::MenuItem("Reset FOV"))
            {
                auto current = cam->GetFOV();
                UndoManager::Execute(
                    Memento(
                        [cam] { cam->SetFOV(); },
                        [cam, current] { cam->SetFOV(current); },
                        "Resetting FOV"));
            }

            ImGui::EndPopup();
        }


        ImGuiHelpers::UndoableSlider<float>(
            [cam]() -> float { return cam->GetFOV(); },
            [cam](float value) { cam->SetFOV(value); },
            "FOV", 0.0f, 100.0f, "Adjust FOV"
        );

        ImGuiHelpers::UndoableSlider<float>(
            [cam]() { return cam->GetNear(); },
            [cam](float value) { cam->SetNear(value); },
            "Near Plane",
            0.1f, 1000.0f,
            "Changing Near Plane");

        ImGuiHelpers::UndoableSlider<float>(
            [cam]() { return cam->GetFar(); },
            [cam](float value) { cam->SetFar(value); },
            "Far Plane",
            1.0f, 10000.0f,
            "Changing Far Plane");


        ImGui::TreePop();
    }
}

void CameraComponentDrawer::DrawOrtho(std::shared_ptr<OrthographicCamera> cam)
{
    if (ImGui::TreeNodeEx("Orthographic Camera Settings", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::BeginPopupContextItem("OrthographicCameraContext"))
        {
            if (ImGui::MenuItem("Reset Near"))
            {
                cam->SetNear();
                Trace("Resetting near")
            }
            if (ImGui::MenuItem("Reset Far"))
            {
                cam->SetFar();
                Trace("Resetting far")
            }

            ImGui::EndPopup();
        }


        ImGuiHelpers::UndoableSlider<float>(
            [cam]() { return cam->GetNear(); },
            [cam](float value) { cam->SetNear(value); },
            "Near Plane",
            0.1f, 1000.0f,
            "Changing Near Plane");

        ImGuiHelpers::UndoableSlider<float>(
            [cam]() { return cam->GetFar(); },
            [cam](float value) { cam->SetFar(value); },
            "Far Plane",
            1.0f, 10000.0f,
            "Changing Far Plane");


        ImGui::TreePop();
    }
}

void CameraComponentDrawer::DrawCameraControls(std::shared_ptr<ICamera> camera)
{
    if (auto cam = std::dynamic_pointer_cast<PerspectiveCamera>(camera))
    {
        DrawPerspective(cam);
    }
    else if (auto cam = std::dynamic_pointer_cast<OrthographicCamera>(camera))
    {
        DrawOrtho(cam);
    }
    else
    {
        ImGui::Text("No camera attached");
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
                            Trace("Removing camera")
                        }
                    }
                    ImGui::EndPopup();
                }

                if (auto camera = cameraComponent->GetCamera())
                {
                    DrawCameraControls(camera);
                }
            }
        }
    }
}
