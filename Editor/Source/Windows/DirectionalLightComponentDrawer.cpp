#include "DirectionalLightComponentDrawer.h"
#include "DirectionalLightComponent.h"
#include "GameObject.h"
#include "imgui.h"
#include "../ImGuiHelpers.h"

DirectionalLightComponentDrawer::DirectionalLightComponentDrawer(std::weak_ptr<DirectionalLightComponent> component) :
    ComponentDrawer(), component(component)
{
}

void DirectionalLightComponentDrawer::Draw()
{
    if (std::shared_ptr<IComponent> sharedComponent = component.lock())
    {
        if (auto lightComponent = std::dynamic_pointer_cast<DirectionalLightComponent>(sharedComponent))
        {
            if (ImGui::CollapsingHeader("Directional Light", ImGuiTreeNodeFlags_DefaultOpen))
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
                        "Deleting Directional light component"
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
                if (ImGui::ColorEdit3("Light Color", colorFloat,
                                      ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoBorder))
                {
                    lightComponent->SetColor(static_cast<Vec4>(colorFloat));
                }
                auto& intensity = lightComponent->intensity;
                ImGuiHelpers::UndoableDrag<float>(
                    [&intensity]() { return intensity; },
                    [lightComponent](float newValue) { lightComponent->intensity = newValue; },
                    std::string("##Intensity").c_str(),
                    "Changed Intensity",
                    0,
                    10,
                    0.1f
                );
            }
        }
    }
}
