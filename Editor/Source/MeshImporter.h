#pragma once
#include <string>
#include <assimp/scene.h>

class Mesh;

class MeshImporter
{
public:
    static Mesh* LoadMesh(std::string path);
    static  void ProcessNode(aiNode* node, const aiScene* scene, Mesh* mesh);
    static  void ProcessMesh(aiMesh* aiMesh, const aiScene* scene, Mesh* mesh);
};
