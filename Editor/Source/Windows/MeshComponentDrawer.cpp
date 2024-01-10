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

#include "BoundsDrawerHelper.h"
#include "MaterialDrawerHelper.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "../MessageBoxWrapper.h"
#include "../Editor.h"
#include "../External/IconsMaterialDesign.h"

MeshComponentDrawer::~MeshComponentDrawer()
{
}

MeshComponentDrawer::MeshComponentDrawer(std::weak_ptr<MeshComponent> component) : ComponentDrawer(),
    component(component), materialDrawerHelper(MaterialDrawerHelper(component))
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
                    auto cachedComponent = meshComponent;

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
                            if (const auto go = cachedComponent->GetGameObject().lock())
                            {
                                go->AddComponent(cachedComponent);

                                if (const auto renderable = std::dynamic_pointer_cast<IRenderableComponent>(cachedComponent))
                                {
                                    if (const auto graphics = IApplication::GetGraphics(); graphics != nullptr)
                                    {
                                        graphics->UpdateRenderable(renderable->GetMaterial(),
                                                                   renderable->GetRenderable());
                                    }
                                }
                            }
                        },
                        "Deleting mesh component"
                    );

                    ImGui::EndPopup();
                }
            }
            std::string scenePath = "";
            if(const auto scene = SceneManager::GetScene().lock())
            {
                scenePath = scene->GetPath();
            }

             ImGuiHelpers::WrappedText("", meshComponent->GetMeshPath() ==""? "Mesh" : std::filesystem::relative(meshComponent->GetMeshPath(), scenePath).string(), ICON_MD_FIND_REPLACE,
                                       std::bind(&MeshComponentDrawer::ChangeMesh, this),meshComponent->GetMeshPath(), true);

            materialDrawerHelper.DrawMaterial();
            if (const auto& renderable = meshComponent->GetRenderable())
            {
                BoundsDrawerHelper::DrawBounds(renderable->GetBounds());
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
            Mesh* mesh = ResourceManager::GetMesh(path.generic_string());
            if (mesh == nullptr)
            {
                MessageBoxWrapper::ShowWarning("Unable to generate mesh", "Unable to generate mesh");
                return;
            }
            Trace("Setting new mesh")
            auto meshRenderable = std::dynamic_pointer_cast<IMeshRenderable>(comp->GetRenderable());

            std::string originalMeshPath = "";
            if (meshRenderable != nullptr)
            {
                originalMeshPath = meshRenderable->GetPath();
            }

            UndoManager::Execute(Memento([comp,mesh]()
                                         {
                                             comp->SetMesh(mesh);
                                         }, [comp,originalMeshPath]()
                                         {
                                             Mesh* mesh = ResourceManager::GetMesh(originalMeshPath);
                                             comp->SetMesh(mesh);
                                         }, "Setting new mesh"));
        }
    }
    else
    {
        MessageBoxWrapper::ShowWarning("Incorrect file type", "Incorrect file type");
    }
}
