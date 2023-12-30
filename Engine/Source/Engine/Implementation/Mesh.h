#pragma once
#include <DirectXCollision.h>
#include <string>
#include <vector>

#include "Vertex.h"

class Mesh
{
public:
    Mesh(std::string path);
    std::vector<Vertex>& GetVerts();
    void SetVerts(std::vector<Vertex> verts);
    std::vector<unsigned int>& GetIndices();
    void SetIndices(std::vector<unsigned int> indices);
    std::string GetPath() { return path; }
    DirectX::BoundingBox GetAABB();
    void SetExtents(Vec3 vec3);

protected:
    std::string path;
    DirectX::BoundingBox aabb;
    std::vector<Vertex> Vertices = {};
    std::vector<unsigned int> Indices = {};
};
