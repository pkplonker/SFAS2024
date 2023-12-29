#include "Mesh.h"

Mesh::Mesh(std::string path)
{
    this->path = path;
}

std::vector<Vertex>& Mesh::GetVerts()
{
    return Vertices;
}

void Mesh::SetVerts(std::vector<Vertex> verts)
{
    this->Vertices = verts;
}

std::vector<unsigned>& Mesh::GetIndices()
{
    return Indices;
}

void Mesh::SetIndices(std::vector<unsigned> indices)
{
    this->Indices = indices;
}
