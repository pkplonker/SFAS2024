#include "MeshSerializer.h"

#include <fstream>
#include <iostream>

#include "Engine/Implementation/Logging/Debug.h"


bool MeshSerializer::Serialize(Mesh* mesh, std::string fullFilePath)
{
    std::ofstream file(fullFilePath, std::ios::binary);

    if (file.is_open())
    {
        unsigned int numVertices = mesh->Vertices.size();
        unsigned int numIndices = mesh->Indices.size();

        file.write(reinterpret_cast<char*>(&numVertices), sizeof(unsigned int));
        file.write(reinterpret_cast<char*>(&numIndices), sizeof(unsigned int));
        file.write(reinterpret_cast<char*>(mesh->Vertices.data()), numVertices * sizeof(Vertex));
        file.write(reinterpret_cast<char*>(mesh->Indices.data()), numIndices * sizeof(unsigned int));

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

        mesh->Vertices.resize(numVertices);
        mesh->Indices.resize(numIndices);

        file.read(reinterpret_cast<char*>(mesh->Vertices.data()), numVertices * sizeof(Vertex));
        file.read(reinterpret_cast<char*>(mesh->Indices.data()), numIndices * sizeof(unsigned int));

        file.close();

        return mesh;
    }

    std::string error = "Failed to open file for reading";
    Trace(error)
    return nullptr;
}
