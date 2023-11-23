#pragma once
#include <string>

#include "Engine/Implementation/Mesh.h"

class MeshSerializer
{
public:
    inline static const std::wstring MESH_EXTENSION = L"smesh";
    static void Serialize(Mesh* mesh, std::wstring path);
    static Mesh* Deserialize(std::wstring path);
};
