#include "MeshComponentDrawer.h"

#include "imgui.h"
#include "../ImGuiHelpers.h"
#include "Engine/Implementation/Helpers.h"
#include "Engine/IMaterial.h"
#include "Engine/IShader.h"
#include "Engine/ITexture.h"
#include "Engine/Implementation/MeshComponent.h"

MeshComponentDrawer::~MeshComponentDrawer()
{
}

MeshComponentDrawer::MeshComponentDrawer(std::weak_ptr<IComponent> component) : ComponentDrawer(
    std::move(component))
{
}

void MeshComponentDrawer::Draw()
{
    if (std::shared_ptr<IComponent> sharedComponent = component.lock())
    {
        if (auto meshComponent = std::dynamic_pointer_cast<MeshComponent>(sharedComponent))
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
            
            ImGuiHelpers::WrappedText("Mesh Path:",meshComponent->GetMeshPath());
            
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
