#pragma once
#include <string>

#include "ImGuiWindow.h"

class Mesh;

class MeshImporterWindow : public EditorWindow
{
public:
    MeshImporterWindow() = default;

    void Draw() override;
    static void Load();
    static std::string SanitisePath(std::string path);
    static void Load(std::string input, std::string output);

    std::string GetName() override
    {
        return "Mesh Importer";
    }
};
