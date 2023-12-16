#pragma once
#include <string>

class Mesh;


class DefaultShapes
{
public:
    static Mesh* GetSphereMesh();
    static Mesh* GetCubeMesh();
    static Mesh* GetPlaneMesh();

private:
    inline static const std::string DEFAULT_SPHERE_MESH_PATH =
        "S:/Users/pkplo/OneDrive/Documents/C++/SFAS2024/Editor/Resource/Mesh/sphere.smesh";
    inline static const std::string DEFAULT_CUBE_MESH_PATH =
        "S:/Users/pkplo/OneDrive/Documents/C++/SFAS2024/Editor/Resource/Mesh/cube.smesh";
    inline static const std::string DEFAULT_PLANE_MESH_PATH =
        "S:/Users/pkplo/OneDrive/Documents/C++/SFAS2024/Editor/Resource/Mesh/plane.smesh";
};
