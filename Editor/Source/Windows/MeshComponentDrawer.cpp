#include "MeshComponentDrawer.h"

#include <functional>

#include "Debug.h"
#include "imgui.h"
#include "MeshSerializer.h"
#include "../FileDialog.h"
#include "../ImGuiHelpers.h"
#include "Engine/IMaterial.h"
#include "Engine/IShader.h"
#include "Engine/ITexture.h"
#include "Engine/Implementation/MeshComponent.h"
#include <filesystem>

#include "ResourceManager.h"
#include "../MessageBoxWrapper.h"
#include "../Editor.h"

MeshComponentDrawer::~MeshComponentDrawer()
{
}

MeshComponentDrawer::MeshComponentDrawer(std::weak_ptr<MeshComponent> component) : component(component),
    ComponentDrawer()
{
}

void MeshComponentDrawer::ChangeMesh()
{
    std::filesystem::path path = FileDialog::OpenFileDialog();
    std::string extension = path.extension().string();
    if (extension == "." + MeshSerializer::MESH_EXTENSION)
    {
        if (auto comp = component.lock())
        {
            auto mesh = ResourceManager::GetMesh(path.generic_string());
            if (mesh == nullptr)
            {
                MessageBoxWrapper::ShowWarning("Unable to generate mesh", "Unable to generate mesh");
                return;
            }
            auto meshRenderable = Editor::GetGraphics()->CreateMeshRenderable(comp->GetMaterial(), mesh);
            comp->SetMesh(meshRenderable);
        }
    }
    else
    {
        Debug("Incorrect file type")
        MessageBoxWrapper::ShowWarning("Incorrect file type", "Incorrect file type");
    }
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

            ImGuiHelpers::WrappedText("Mesh Path:", meshComponent->GetMeshPath(),
                                      std::bind(&MeshComponentDrawer::ChangeMesh, this));

            if (const auto mat = meshComponent->GetMaterial())
            {
                if (const auto shader = mat->GetShader())
                {
                    const auto shaderPath = shader->GetPath();
                    if (shaderPath != L"")
                    {
                        ImGuiHelpers::WrappedText("Shader Path:", shaderPath);
                    }
                }
                if (const auto tex = mat->GetTexture())
                {
                    const auto texturePath = tex->GetPath();
                    if (texturePath != L"")
                    {
                        ImGuiHelpers::WrappedText("Texture Path:", texturePath);
                    }
                }
                auto color = mat->GetColor();
                ImGui::Text("Color:");
                ImGui::SameLine();
                if (ImGui::ColorEdit4("", reinterpret_cast<float*>(&color), ImGuiColorEditFlags_Float))
                {
                    mat->SetColor(color);
                }
            }
        }
    }
}
