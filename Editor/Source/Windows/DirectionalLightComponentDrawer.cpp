#include "DirectionalLightComponentDrawer.h"
#include "DirectionalLightComponent.h"
#include "GameObject.h"
#include "imgui.h"
#include "../ImGuiHelpers.h"

DirectionalLightComponentDrawer::DirectionalLightComponentDrawer(std::weak_ptr<DirectionalLightComponent> component) : ComponentDrawer(), component(component)
{
}

void DirectionalLightComponentDrawer::Draw()
{
    if (std::shared_ptr<IComponent> sharedComponent = component.lock())
    {
        if (auto meshComponent = std::dynamic_pointer_cast<DirectionalLightComponent>(sharedComponent))
        {
            if (ImGui::CollapsingHeader("Directional Light Component", ImGuiTreeNodeFlags_DefaultOpen))
            {
                if (ImGui::BeginPopupContextItem("DirectionalLightComponentContext"))
                {
                    auto cachedComponent = meshComponent;

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
            }
        }
    }
}
