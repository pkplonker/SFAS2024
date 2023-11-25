#include "MeshComponentDrawer.h"

#include "imgui.h"
#include "Engine/Helpers.h"
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
            ImGui::Text("Mesh Path:");
            ImGui::SameLine();
            ImGui::Text(meshComponent->GetMeshPath().c_str());
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
            }
        }
    }
}
