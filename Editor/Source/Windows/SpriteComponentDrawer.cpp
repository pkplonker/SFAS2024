#include "SpriteComponentDrawer.h"

#include "Helpers.h"
#include "IMaterial.h"
#include "imgui.h"
#include "IShader.h"
#include "ITexture.h"
#include "MaterialDrawerHelper.h"
#include "SpriteComponent.h"
#include "../ImGuiHelpers.h"

SpriteComponentDrawer::~SpriteComponentDrawer()
{
}

SpriteComponentDrawer::SpriteComponentDrawer(std::weak_ptr<SpriteComponent> component) : component(component), materialDrawerHelper(MaterialDrawerHelper(component))
{
}

void SpriteComponentDrawer::Draw()
{
    if (std::shared_ptr<IComponent> sharedComponent = component.lock())
    {
        if (auto component = std::dynamic_pointer_cast<SpriteComponent>(sharedComponent))
        {
            if (ImGui::CollapsingHeader("Sprite Component", ImGuiTreeNodeFlags_DefaultOpen))
            {
                if (ImGui::BeginPopupContextItem("MeshComponentContext"))
                {
                    if (ImGui::MenuItem("Delete component"))
                    {
                        if (auto gameobject = component->GetGameObject().lock())
                        {
                            gameobject->RemoveComponent(component);
                        }
                    }
                    ImGui::EndPopup();
                }
            }

            materialDrawerHelper.DrawMaterial();
        }
    }
}
