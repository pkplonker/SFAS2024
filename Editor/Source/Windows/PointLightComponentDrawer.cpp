#include "PointLightComponentDrawer.h"
#include "GameObject.h"
#include "imgui.h"
#include "../ImGuiHelpers.h"

void PointLightComponentDrawer::Draw()
{
    if (std::shared_ptr<IComponent> sharedComponent = component.lock())
    {
        if (auto lightComponent = std::dynamic_pointer_cast<PointLightComponent>(sharedComponent))
        {
            if (ImGui::CollapsingHeader("Point Light", ImGuiTreeNodeFlags_DefaultOpen))
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
                        "Deleting Point light component"
                    );

                    ImGui::EndPopup();
                }
                

                ImGui::Text("Light Color");
                ImGui::SameLine();
                ImGuiHelpers::UndoableColorEdit<DirectX::XMFLOAT4>(
                    [&]() { return lightComponent->GetLight().Color; },
                    [lightComponent](DirectX::XMFLOAT4 newValue) { lightComponent->GetLight().Color = newValue; },
                    "##LightColor",
                    "Change Light Color"
                );
                
                ImGui::Text("Constant Attenuation");
                ImGui::SameLine();
                ImGuiHelpers::UndoableDrag<float>(
                    [&]() { return lightComponent->GetLight().ConstantAttenuation; },
                    [lightComponent](float newValue) { lightComponent->GetLight().ConstantAttenuation = newValue; },
                    "##LightColor",
                    "Change Light ConstantAttenuation",0,10,0.01f
                );
                ImGui::Text("Linear Attenuation");
                ImGui::SameLine();
                ImGuiHelpers::UndoableDrag<float>(
                    [&]() { return lightComponent->GetLight().LinearAttenuation; },
                    [lightComponent](float newValue) { lightComponent->GetLight().LinearAttenuation = newValue; },
                    "##Linear Attenuation",
                    "Change Light Linear Attenuation",0,10,0.01f
                );
                ImGui::Text("Quadratic Attenuation");
                ImGui::SameLine();
                ImGuiHelpers::UndoableDrag<float>(
                    [&]() { return lightComponent->GetLight().QuadraticAttenuation; },
                    [lightComponent](float newValue) { lightComponent->GetLight().QuadraticAttenuation = newValue; },
                    "##Quadratic Attenuation",
                    "Change Light Quadratic Attenuation",0,10,0.01f
                );
            }
        }
    }
}
