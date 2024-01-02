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
#include "../FileOpener.h"
#include "../MessageBoxWrapper.h"
#include "DirectX11/DirectX11Texture.h"
#include "../External/IconsMaterialDesign.h"

MaterialDrawerHelper::MaterialDrawerHelper(std::weak_ptr<IRenderableComponent> component) : component(component)
{
}

void MaterialDrawerHelper::DrawMaterial()
{
    ImGui::Separator();
    if (auto comp = component.lock())
    {
        const auto mat = comp->GetMaterial();
        if (mat != nullptr)
        {
            std::wstring shaderPath;
            const auto shader = mat->GetShader();
            if (shader != nullptr)
            {
                shaderPath = shader->GetPath();
            }
            std::vector<ImGuiHelpers::ButtonData> buttons;

            buttons.emplace_back(ImGuiHelpers::ButtonData(ICON_MD_FIND_REPLACE, std::bind(&MaterialDrawerHelper::ChangeShader, this),true));
            buttons.emplace_back(ImGuiHelpers::ButtonData(ICON_MD_EDIT_DOCUMENT, std::bind(&MaterialDrawerHelper::OpenShader, this),true));
            buttons.emplace_back(ImGuiHelpers::ButtonData(ICON_MD_REFRESH, std::bind(&IShader::Reload, shader),true));
            
            ImGuiHelpers::WrappedText("", shaderPath == L"" ? L"Shader" : shaderPath, buttons);

            std::wstring texturePath;
            ITexture* tex = mat->GetTexture();
            if (tex != nullptr)
            {
                texturePath = tex->GetPath();
            }

            ImGuiHelpers::WrappedText("", texturePath == L"" ? L"Texture" : texturePath,ICON_MD_FIND_REPLACE,
                                      std::bind(&MaterialDrawerHelper::ChangeTexture, this));
            
            
            if (tex != nullptr)
            {
                if (auto dxTex = dynamic_cast<DirectX11Texture*>(tex))
                {
                    ImGui::Separator();
                    ImGui::Image(dxTex->GetTextureView(), ImVec2(200, 200));
                    std::stringstream stream;
                    stream << std::fixed << std::setprecision(2) << "Texture size:" << dxTex->GetWidth() << " * " << dxTex
                        ->GetHeight();
                    ImGui::Text(stream.str().c_str());
                    if (ImGui::IsItemHovered())
                    {
                        ImGui::BeginTooltip();
                        auto size = ImGui::GetWindowSize();
                        auto factor = 0.9f;
                        ImGui::Image(dxTex->GetTextureView(), ImVec2(size.x * factor, size.y * factor));
                        ImGui::EndTooltip();
                    }

                    if (ImGui::IsItemClicked())
                    {
                        ImGui::OpenPopup("##TexturePopup");
                    }

                    if (ImGui::BeginPopupModal("##TexturePopup", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
                    {
                        auto size = ImGui::GetMainViewport()->Size;
                        auto factor = 0.5f;
                        ImGui::Image(dxTex->GetTextureView(), ImVec2(size.x * factor, size.y * factor));
                        if (ImGui::IsItemClicked())
                        {
                            ImGui::CloseCurrentPopup();
                        }
                        ImGui::EndPopup();
                    }
                    ImGui::Separator();
                }
            }


            ImGui::Text("Color:");
            ImGui::SameLine();
            Vec4 color = static_cast<Vec4>(mat->GetColor());

            ImGuiHelpers::UndoableColorEdit<Vec4>(
                [&]() { return static_cast<Vec4>(mat->GetColor()); },
                [mat](Vec4 newColor) { mat->SetColor(newColor); },
                "##MaterialColor",
                "Change Material Color",
                ImGuiColorEditFlags_Float
            );

            auto sb = mat->GetIsSkybox();
            if (ImGui::Checkbox("Is Skybox", &sb))
            {
                mat->SetIsSkyBox(sb);
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

            auto originalMaterial = std::make_shared<std::shared_ptr<IMaterial>>(comp->GetMaterial());
            UndoManager::Execute(Memento(
                [comp, shader, originalMaterial]()
                {
                    auto material = Editor::GetGraphics()->CreateMaterial(shader, (*originalMaterial)->GetTexture());
                    material->SetColor((*originalMaterial)->GetColor());
                    material->SetIsSkyBox((*originalMaterial)->GetIsSkybox());
                    comp->SetMaterial(material);
                },
                [comp, originalMaterial]()
                {
                    if (*originalMaterial != nullptr)
                    {
                        comp->SetMaterial(originalMaterial->get());
                    }
                },
                "Changed Shader"
            ));

            Trace("Setting new shader/material");
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
            auto originalMaterial = std::make_shared<std::shared_ptr<IMaterial>>(comp->GetMaterial());
            UndoManager::Execute(Memento(
                [comp, texture, originalMaterial]()
                {
                    auto material = Editor::GetGraphics()->CreateMaterial((*originalMaterial)->GetShader(), texture);
                    material->SetColor((*originalMaterial)->GetColor());
                    material->SetIsSkyBox((*originalMaterial)->GetIsSkybox());
                    comp->SetMaterial(material);
                },
                [comp, originalMaterial]()
                {
                    if (*originalMaterial != nullptr)
                    {
                        comp->SetMaterial(originalMaterial->get());
                    }
                },
                "Changed Shader"
            ));
            UndoManager::Execute(Memento([comp, texture]()
                                         {
                                         }
                                         , []()
                                         {
                                         }, "Changed Shader"));

            Trace("Setting new texture/material")
        }
    }
    else
    {
        MessageBoxWrapper::ShowWarning("Incorrect file type", "Incorrect file type");
    }
}

void MaterialDrawerHelper::OpenShader() const
{
    if (const auto comp = component.lock())
    {
        if (const auto mat = comp->GetMaterial())
        {
            if (const auto shader = mat->GetShader())
            {
                FileOpener::Open(shader->GetPath());
            }
        }
    }
}
