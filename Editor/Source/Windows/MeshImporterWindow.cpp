#include "MeshImporterWindow.h"
#include "imgui.h"
#include "Engine/MeshSerializer.h"
#include "../MeshImporter.h"
#include "../FileDialog.h"

void MeshImporterWindow::Draw()
{
    ImGui::Begin(GetName().c_str());
    if (ImGui::Button("Select File"))
    {
        selectedFile = FileDialog::OpenFileDialog();
        outputPath = selectedFile;
    }
    ImGui::SameLine();
    if (ImGui::Button("Serialize Mesh"))
    {
        outputPath = FileDialog::SaveFileDialog();
        if (!outputPath.empty())
        {
            outputPath += "." + MeshSerializer::MESH_EXTENSION;
            Mesh* mesh = MeshImporter::LoadMesh(selectedFile);
            MeshSerializer::Serialize(mesh, outputPath);
        }
    }
    ImGui::End();
}
