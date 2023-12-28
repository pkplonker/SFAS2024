#include "SpriteComponentDrawer.h"

#include "Helpers.h"
#include "IApplication.h"
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

SpriteComponentDrawer::SpriteComponentDrawer(std::weak_ptr<SpriteComponent> component) : component(component),
    materialDrawerHelper(MaterialDrawerHelper(component))
{
}

void SpriteComponentDrawer::Draw()
{
    if (std::shared_ptr<IComponent> sharedComponent = component.lock())
    {
        if (auto spriteComponent = std::dynamic_pointer_cast<SpriteComponent>(sharedComponent))
        {
            if (ImGui::CollapsingHeader("Sprite Component", ImGuiTreeNodeFlags_DefaultOpen))
            {
                if (ImGui::BeginPopupContextItem("SpriteComponentContext"))
                {
                    auto cachedComponent = spriteComponent;

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
                                auto renderable = std::dynamic_pointer_cast<IRenderableComponent>(cachedComponent);
                                if (renderable)
                                {
                                    auto graphics = IApplication::GetGraphics();
                                    if (graphics != nullptr)
                                    {
                                        graphics->UpdateRenderable(renderable->GetMaterial(),
                                                                   renderable->GetRenderable());
                                    }
                                }
                            }
                        },
                        "Deleting sprite component"
                    );

                    ImGui::EndPopup();
                }
            }

            materialDrawerHelper.DrawMaterial();
        }
    }
}
