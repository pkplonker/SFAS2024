#include "MeshComponentDrawer.h"

#include <functional>

#include "Engine/Implementation/Logging/Debug.h"
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

            const auto mat = meshComponent->GetMaterial();
            if (mat != nullptr)
            {
                std::wstring shaderPath;
                if (const auto shader = mat->GetShader())
                {
                    shaderPath = shader->GetPath();
                }


                ImGuiHelpers::WrappedText("Shader Path:", shaderPath,
                                          std::bind(&MeshComponentDrawer::ChangeShader, this));

                std::wstring texturePath;
                if (const auto tex = mat->GetTexture())
                {
                    texturePath = tex->GetPath();
                }

                ImGuiHelpers::WrappedText("Texture Path:", texturePath,
                                          std::bind(&MeshComponentDrawer::ChangeTexture, this));


                auto color = mat->GetColor();
                ImGui::Text("Color:");
                ImGui::SameLine();
                if (ImGui::ColorEdit4("", reinterpret_cast<float*>(&color), ImGuiColorEditFlags_Float))
                {
                    mat->SetColor(color);
                }
            }
            else
            {
                if (ImGui::Button("Create Material"))
                {
                    Trace("Creating material for mesh component")
                    auto mat = IApplication::GetGraphics()->CreateMaterial(nullptr, nullptr);
                    meshComponent->SetMaterial(mat);
                }
            }
        }
    }
}


void MeshComponentDrawer::ChangeMesh()
{
    std::filesystem::path path = FileDialog::OpenFileDialog();
    if (path.empty())
    {
        Trace("Closed dialog without selecting path")
        return;
    }
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
            Trace("Setting new mesh")
            comp->SetMesh(mesh);
        }
    }
    else
    {
        MessageBoxWrapper::ShowWarning("Incorrect file type", "Incorrect file type");
    }
}

void MeshComponentDrawer::ChangeShader()
{
    std::filesystem::path path = FileDialog::OpenFileDialog();
    if (path.empty())
    {
        Trace("Closed dialog without selecting path")
        return;
    }
    std::string extension = path.extension().string();
    if (extension == ".fx")
    {
        if (auto comp = component.lock())
        {
            auto shader = ResourceManager::GetShader(path.wstring());
            if (shader == nullptr)
            {
                MessageBoxWrapper::ShowWarning("Unable to generate shader", "Unable to generate shader");
                return;
            }
            auto material = Editor::GetGraphics()->CreateMaterial(shader, comp->GetMaterial()->GetTexture());
            material->SetColor(comp->GetMaterial()->GetColor()); // need to implement updating existing material
            comp->SetMaterial(material);
            Trace("Setting new shader/material")
        }
    }
    else
    {
        MessageBoxWrapper::ShowWarning("Incorrect file type", "Incorrect file type");
    }
}

void MeshComponentDrawer::ChangeTexture()
{
    std::filesystem::path path = FileDialog::OpenFileDialog();
    if (path.empty())
    {
        Trace("Closed dialog without selecting path")
        return;
    }
    std::string extension = path.extension().string();
    if (extension == ".dds")
    {
        if (auto comp = component.lock())
        {
            auto texture = ResourceManager::GetTexture(path.wstring());
            if (texture == nullptr)
            {
                MessageBoxWrapper::ShowWarning("Unable to generate texture", "Unable to generate texture");
                return;
            }
            auto material = IApplication::GetGraphics()->CreateMaterial(comp->GetMaterial()->GetShader(), texture);
            material->SetColor(comp->GetMaterial()->GetColor()); // need to implement updating existing material
            comp->SetMaterial(material);
            Trace("Setting new texture/material")
        }
    }
    else
    {
        MessageBoxWrapper::ShowWarning("Incorrect file type", "Incorrect file type");
    }
}
