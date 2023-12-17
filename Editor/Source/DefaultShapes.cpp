#include "DefaultShapes.h"

#include "ResourceManager.h"

Mesh* DefaultShapes::GetSphereMesh()
{
    return ResourceManager::GetMesh(DEFAULT_SPHERE_MESH_PATH);
}

Mesh* DefaultShapes::GetCubeMesh()
{
    return ResourceManager::GetMesh(DEFAULT_CUBE_MESH_PATH);
}

Mesh* DefaultShapes::GetPlaneMesh()
{
    return ResourceManager::GetMesh(DEFAULT_PLANE_MESH_PATH);
}
