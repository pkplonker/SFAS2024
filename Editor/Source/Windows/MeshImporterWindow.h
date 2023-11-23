#pragma once
#include <string>

#include "ImGuiWindow.h"

class Mesh;

class MeshImporterWindow : public EditorWindow
{
public:
    MeshImporterWindow() = default;

    void Draw() override;

    std::string GetName() override
    {
        return "Mesh Importer";
    }

private:
    std::string selectedFile;
    std::string outputPath;
};
