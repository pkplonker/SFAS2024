#include "MaterialDrawerHelper.h"

#include <filesystem>
#include <functional>
#include <string>

#include "imgui.h"
#include "IRenderableComponent.h"
#include "ResourceManager.h"
#include "Logging/Debug.h"
#include "../ImGuiHelpers.h"
#include "../FileDialog.h"
#include "../Editor.h"
#include "../MessageBoxWrapper.h"
MaterialDrawerHelper::MaterialDrawerHelper(std::weak_ptr<IRenderableComponent> component) : component(component)
{
    
}

void MaterialDrawerHelper::DrawMaterial()
{
    if (auto comp = component.lock())
    {
        const auto mat = comp->GetMaterial();
        if (mat != nullptr)
        {
            std::wstring shaderPath;
            if (const auto shader = mat->GetShader())
            {
                shaderPath = shader->GetPath();
            }


            ImGuiHelpers::WrappedText("Shader Path:", shaderPath,
                                      std::bind(&MaterialDrawerHelper::ChangeShader, this));

            std::wstring texturePath;
            if (const auto tex = mat->GetTexture())
            {
                texturePath = tex->GetPath();
            }

            ImGuiHelpers::WrappedText("Texture Path:", texturePath,
                                      std::bind(&MaterialDrawerHelper::ChangeTexture, this));


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
                Trace("Creating material for component")
                auto mat = ResourceManager::GetMaterial();
                comp->SetMaterial(mat);
            }
        }
    }
}


void MaterialDrawerHelper::ChangeShader()
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

void MaterialDrawerHelper::ChangeTexture()
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
