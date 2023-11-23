#pragma once
#include <string>

#include "Engine/Implementation/Mesh.h"

class MeshSerializer
{
public:
    inline static const std::string MESH_EXTENSION = "smesh";
    static void Serialize(Mesh* mesh, std::string path);
    static Mesh* Deserialize(std::string path);
};
