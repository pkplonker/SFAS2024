#include "SpotLightComponentDrawer.h"
#include "GameObject.h"
#include "imgui.h"
#include "../ImGuiHelpers.h"

void SpotLightComponentDrawer::Draw()
{
    if (std::shared_ptr<IComponent> sharedComponent = component.lock())
    {
        if (auto lightComponent = std::dynamic_pointer_cast<SpotLightComponent>(sharedComponent))
        {
            if (ImGui::CollapsingHeader("Spot Light", ImGuiTreeNodeFlags_DefaultOpen))
            {
                if (ImGui::BeginPopupContextItem("DirectionalLightComponentContext"))
                {
                    auto cachedComponent = lightComponent;

                    ImGuiHelpers::UndoableMenuItemAction(
                        "Delete component",
                        [cachedComponent]()
                        {
                            if (auto go = cachedComponent->GetGameObject().lock())
                            {
                                go->RemoveComponent(cachedComponent);
                            }
                        },
                        [cachedComponent]()
                        {
                            if (auto go = cachedComponent->GetGameObject().lock())
                            {
                                go->AddComponent(cachedComponent);
                            }
                        },
                        "Deleting Spot light component"
                    );

                    ImGui::EndPopup();
                }
                auto color = static_cast<Vec4>(lightComponent->GetColor());
                float colorFloat[3];
                colorFloat[0] = color.X();
                colorFloat[1] = color.Y();
                colorFloat[2] = color.Z();

                ImGui::Text("Light Color");
                ImGui::SameLine();
                ImGuiHelpers::UndoableColorEdit<Vec3>(
                    [&]() { return static_cast<Vec3>(lightComponent->GetColor()); },
                    [lightComponent](Vec3 newColor) { lightComponent->SetColor(newColor); },
                    "##LightColor",
                    "Change Light Color"
                );
                
                ImGui::Text("Light Intensity");
                ImGui::SameLine();
                auto intensity = lightComponent->GetIntensity();
                ImGuiHelpers::UndoableDrag<float>(
                    [&intensity]() { return intensity; },
                    [lightComponent](float newValue) { lightComponent->SetIntensity(newValue); },
                    std::string("##Intensity").c_str(),
                    "Changed Intensity",
                    0,
                    10,
                    0.1f
                );
                
                ImGui::Text("Inner Cone Angle");
                ImGui::SameLine();
                auto inner = lightComponent->GetInnerCone();
                ImGuiHelpers::UndoableDrag<float>(
                    [&inner]() { return inner; },
                    [lightComponent](float newValue) { lightComponent->SetInnerCone(newValue); },
                    std::string("##Inner Cone Angle").c_str(),
                    "Changed inner cone angle",
                    0,
                    10,
                    0.1f
                );
                
                ImGui::Text("Outer Cone Angle");
                ImGui::SameLine();
                auto outer = lightComponent->GetOuterCone();
                ImGuiHelpers::UndoableDrag<float>(
                    [&outer]() { return outer; },
                    [lightComponent](float newValue) { lightComponent->SetOuterCone(newValue); },
                    std::string("##Outer Cone Angle").c_str(),
                    "Changed outer cone angle",
                    0,
                    10,
                    0.1f
                );
            }
        }
    }
}
