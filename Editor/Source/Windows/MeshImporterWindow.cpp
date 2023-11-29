#include "MeshImporterWindow.h"

#include "Debug.h"
#include "imgui.h"
#include "Engine/MeshSerializer.h"
#include "../MeshImporter.h"
#include "../FileDialog.h"
#include "../MessageBoxWrapper.h"

void MeshImporterWindow::Draw()
{
    ImGui::Begin(GetName().c_str());

    if (ImGui::Button("Select File"))
    {
        Load();
    }

    ImGui::End();
}

void MeshImporterWindow::Load()
{
    const auto inputPath = FileDialog::OpenFileDialog();
    if (inputPath.empty())
    {
        return;
    }
    Load(inputPath, FileDialog::SaveFileDialog());
}

std::string MeshImporterWindow::SanitisePath(std::string path)
{
    if (!path.empty())
    {
        const auto extension = path.substr(path.find_last_of(".") + 1);
        if (extension.length() == 0)
        {
            path += "." + MeshSerializer::MESH_EXTENSION;
        }
        else if (extension != MeshSerializer::MESH_EXTENSION)
        {
            path = path.substr(0, path.find_last_of(".")) + "." + MeshSerializer::MESH_EXTENSION;
        }
    }
    return path;
}

void MeshImporterWindow::Load(std::string input, std::string output)
{
    output = SanitisePath(output);

    Mesh* mesh = MeshImporter::LoadMesh(input);
    if (mesh)
    {
        if (MeshSerializer::Serialize(mesh, output))
        {
            Debug("Serialized mesh")
        }
        else
        {
            MessageBoxWrapper::ShowWarning("Failed to serialize mesh", "Failed to serialize mesh");
            Debug("Serialized mesh failed")
        }
    }
    else
    {
        MessageBoxWrapper::ShowWarning("Failed to load mesh", "Failed to load mesh");
        Debug("Failed to load mesh")
    }
}
