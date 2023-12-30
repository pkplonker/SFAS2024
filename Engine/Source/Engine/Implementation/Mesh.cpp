#include "Mesh.h"

#include <iostream>

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


DirectX::BoundingBox Mesh::GetAABB()
{
    return aabb;
}

void Mesh::SetExtents(Vec3 extents)
{
    aabb = DirectX::BoundingBox(DirectX::XMFLOAT3(0,0,0), extents);
}
