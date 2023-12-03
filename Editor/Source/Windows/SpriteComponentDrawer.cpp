#include "SpriteComponentDrawer.h"

#include "Helpers.h"
#include "IMaterial.h"
#include "imgui.h"
#include "IShader.h"
#include "ITexture.h"
#include "SpriteComponent.h"
#include "../ImGuiHelpers.h"

SpriteComponentDrawer::~SpriteComponentDrawer()
{
}

SpriteComponentDrawer::SpriteComponentDrawer(std::weak_ptr<SpriteComponent> component) : component(component)
{
}

void SpriteComponentDrawer::Draw()
{
    if (std::shared_ptr<IComponent> sharedComponent = component.lock())
    {
        if (auto meshComponent = std::dynamic_pointer_cast<SpriteComponent>(sharedComponent))
        {
            if (ImGui::CollapsingHeader("Sprite Component", ImGuiTreeNodeFlags_DefaultOpen))
            {
                if (ImGui::BeginPopupContextItem("MeshComponentContext"))
                {
                    if (ImGui::MenuItem("Delete component"))
                    {
                        if (auto gameobject = meshComponent->GetGameObject().lock())
                        {
                            gameobject->RemoveComponent(meshComponent);
                        }
                    }
                    ImGui::EndPopup();
                }
            }
            
            if (const auto mat = meshComponent->GetMaterial())
            {
                if (const auto shader = mat->GetShader())
                {
                    const auto shaderPath = shader->GetPath();
                    if (shaderPath != L"")
                    {
                        ImGuiHelpers::WrappedText("Shader Path:",shaderPath);
                    }
                }
                if (const auto tex = mat->GetTexture())
                {
                    const auto texturePath = tex->GetPath();
                    if (texturePath != L"")
                    {
                        ImGuiHelpers::WrappedText("Texture Path:",texturePath);
                    }
                }
                auto color = mat->GetColor();
                ImGui::Text("Color:");
                ImGui::SameLine();
                if(ImGui::ColorEdit4("", reinterpret_cast<float*>(&color), ImGuiColorEditFlags_Float ))
                {
                    mat->SetColor(color);
                }
            
            }
        }
    }
}
