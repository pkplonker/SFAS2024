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

#include "MaterialDrawerHelper.h"
#include "ResourceManager.h"
#include "../MessageBoxWrapper.h"
#include "../Editor.h"

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
                        "Deleting camera component"
                    );

                    ImGui::EndPopup();
                }
            }


            ImGuiHelpers::WrappedText("Mesh Path:", meshComponent->GetMeshPath(), "Replace",
                                      std::bind(&MeshComponentDrawer::ChangeMesh, this));

            materialDrawerHelper.DrawMaterial();
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
            auto originalMeshPath = meshRenderable->GetPath();
            if (meshRenderable)
            {
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
    }
    else
    {
        MessageBoxWrapper::ShowWarning("Incorrect file type", "Incorrect file type");
    }
}
