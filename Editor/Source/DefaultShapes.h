#pragma once
#include <string>

#include "Helpers.h"

class Mesh;


class DefaultShapes
{
public:
    static Mesh* GetSphereMesh();
    static Mesh* GetCubeMesh();
    static Mesh* GetPlaneMesh();

private:
    inline static std::string DEFAULT_SPHERE_MESH_PATH =
        Helpers::ToAbsolutePath("Resource/Mesh/sphere.smesh");
    inline static std::string DEFAULT_CUBE_MESH_PATH =
        Helpers::ToAbsolutePath("Resource/Mesh/cube.smesh");
    inline static std::string DEFAULT_PLANE_MESH_PATH =
        Helpers::ToAbsolutePath("Resource/Mesh/plane.smesh");
};
