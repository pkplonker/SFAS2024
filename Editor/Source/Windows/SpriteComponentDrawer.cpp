#include "SpriteComponentDrawer.h"

#include "Helpers.h"
#include "IMaterial.h"
#include "imgui.h"
#include "IShader.h"
#include "ITexture.h"
#include "SpriteComponent.h"

SpriteComponentDrawer::~SpriteComponentDrawer()
{
}

SpriteComponentDrawer::SpriteComponentDrawer(std::weak_ptr<IComponent> component) : ComponentDrawer(
    std::move(component))
{
}

void SpriteComponentDrawer::Draw()
{
    if (std::shared_ptr<IComponent> sharedComponent = component.lock())
    {
        if (auto meshComponent = std::dynamic_pointer_cast<SpriteComponent>(sharedComponent))
        {
            if (ImGui::CollapsingHeader("Mesh Component", ImGuiTreeNodeFlags_DefaultOpen))
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
                        ImGui::Text("Shader Path:");
                        ImGui::SameLine();
                        ImGui::Text(Helpers::WideStringToString(shaderPath).c_str());
                    }
                }
                if (const auto tex = mat->GetTexture())
                {
                    const auto texturePath = tex->GetPath();
                    if (texturePath != L"")
                    {
                        ImGui::Text("Texture Path:");
                        ImGui::SameLine();
                        ImGui::Text(Helpers::WideStringToString(texturePath).c_str());
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
