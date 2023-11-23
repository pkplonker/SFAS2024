#pragma once
#include <string>
#include <assimp/scene.h>

class Mesh;

class MeshImporter
{
public:
    MeshImporter();
    Mesh* LoadMesh(std::string path);
    void ProcessNode(aiNode* node, const aiScene* scene, Mesh* mesh);
    void ProcessMesh(aiMesh* aiMesh, const aiScene* scene, Mesh* mesh);
};
