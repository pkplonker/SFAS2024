#include "MeshSerializer.h"

#include <fstream>
#include <iostream>

#include "Engine/Implementation/Debug.h"


void MeshSerializer::Serialize(Mesh* mesh, std::wstring fullFilePath)
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
        return;
    }

    std::string error = "Failed to open file for writing";
    Debug(error)
}

Mesh* MeshSerializer::Deserialize(std::wstring fullFilePath)
{
    std::ifstream file(fullFilePath, std::ios::binary);

    if (file.is_open())
    {
        Mesh* mesh = new Mesh();

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
    Debug(error)
    return nullptr;
}
