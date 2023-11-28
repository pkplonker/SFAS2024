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
        selectedFile = FileDialog::OpenFileDialog();

        outputPath = FileDialog::SaveFileDialog();
        if (!outputPath.empty())
        {
            const auto extension = outputPath.substr(outputPath.find_last_of(".") + 1);
            if (extension.length() == 0)
            {
                outputPath += "." + MeshSerializer::MESH_EXTENSION;
            }
            else if (extension != MeshSerializer::MESH_EXTENSION)
            {
                outputPath = outputPath.substr(0, outputPath.find_last_of(".")) + "." + MeshSerializer::MESH_EXTENSION;
            }

            Mesh* mesh = MeshImporter::LoadMesh(selectedFile);
            if (mesh)
            {
                if (MeshSerializer::Serialize(mesh, outputPath))
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
    }
    ImGui::End();
}
