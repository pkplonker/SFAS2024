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
            ImGuiHelpers::UndoableMenuItemValue<float>(
                "Reset Near",
                [cam]() { return cam->GetNear(); },
                [cam](float value) { cam->SetNear(value); },
                0.0f,
                "Resetting near"
            );

            ImGuiHelpers::UndoableMenuItemValue<float>(
                "Reset Far",
                [cam]() { return cam->GetFar(); },
                [cam](float value) { cam->SetFar(value); },
                0.0f,
                "Resetting far"
            );

            ImGuiHelpers::UndoableMenuItemValue<float>(
                "Reset FOV",
                [cam]() { return cam->GetFOV(); },
                [cam](float value) { cam->SetFOV(value); },
                0.0f,
                "Resetting FOV"
            );


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
            ImGuiHelpers::UndoableMenuItemValue<float>(
                "Reset Near",
                [cam]() { return cam->GetNear(); },
                [cam](float value) { cam->SetNear(value); },
                0.0f,
                "Resetting near"
            );

            ImGuiHelpers::UndoableMenuItemValue<float>(
                "Reset Far",
                [cam]() { return cam->GetFar(); },
                [cam](float value) { cam->SetFar(value); },
                0.0f,
                "Resetting far"
            );
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
                    if (auto gameobject = cameraComponent->GetGameObject().lock())
                    {
                        ImGuiHelpers::UndoableMenuItemAction(
                            "Delete component",
                            [cameraComponent]()
                            {
                                auto go = cameraComponent->GetGameObject().lock();
                                go->RemoveComponent(cameraComponent);
                            },
                            [cameraComponent, gameobject]()
                            {
                                if (gameobject)
                                {
                                    gameobject->AddComponent(cameraComponent);
                                }
                            },
                            "Deleting camera component"
                        );
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
