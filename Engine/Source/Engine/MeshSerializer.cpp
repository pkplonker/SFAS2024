#include "MeshSerializer.h"

#include <fstream>
#include <iostream>

#include "Engine/Implementation/Logging/Debug.h"


bool MeshSerializer::Serialize(Mesh* mesh, std::string fullFilePath)
{
    std::ofstream file(fullFilePath, std::ios::binary);
    // need to look at RAII. No finally block in C++ :/
    if (file.is_open())
    {
        unsigned int numVertices = mesh->GetVerts().size();
        unsigned int numIndices = mesh->GetIndices().size();
        file.write(reinterpret_cast<char*>(&numVertices), sizeof(unsigned int));
        file.write(reinterpret_cast<char*>(&numIndices), sizeof(unsigned int));
        file.write(reinterpret_cast<char*>(mesh->GetVerts().data()), numVertices * sizeof(Vertex));
        file.write(reinterpret_cast<char*>(mesh->GetIndices().data()), numIndices * sizeof(unsigned int));
        const auto extents = mesh->GetAABB().Extents;
        file.write(reinterpret_cast<const char*>(&extents), sizeof(extents));

        file.close();
        return true;
    }

    Warning("Failed to open file for writing")
    return false;
}

Mesh* MeshSerializer::Deserialize(std::string fullFilePath)
{
    std::ifstream file(fullFilePath, std::ios::binary);

    if (file.is_open())
    {
        Mesh* mesh = new Mesh(fullFilePath);

        unsigned int numVertices, numIndices;
        file.read(reinterpret_cast<char*>(&numVertices), sizeof(unsigned int));
        file.read(reinterpret_cast<char*>(&numIndices), sizeof(unsigned int));

        mesh->GetVerts().resize(numVertices);
        mesh->GetIndices().resize(numIndices);

        file.read(reinterpret_cast<char*>(mesh->GetVerts().data()), numVertices * sizeof(Vertex));
        file.read(reinterpret_cast<char*>(mesh->GetIndices().data()), numIndices * sizeof(unsigned int));
        auto extents = mesh->GetAABB().Extents;
        file.read(reinterpret_cast<char*>(&extents), sizeof(extents));
        mesh->SetExtents(extents);
        file.close();

        return mesh;
    }

    std::string error = "Failed to open file for reading";
    Trace(error)
    return nullptr;
}
