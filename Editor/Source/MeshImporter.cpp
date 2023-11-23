#include "MeshImporter.h"

#include "Engine/Implementation/Debug.h"
#include "Engine/Implementation/Mesh.h"
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Importer.hpp"

MeshImporter::MeshImporter()
{
}

Mesh* MeshImporter::LoadMesh(std::string path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
                                             aiProcess_CalcTangentSpace |
                                             aiProcess_Triangulate |
                                             aiProcess_JoinIdenticalVertices |
                                             aiProcess_SortByPType);

    if (nullptr == scene)
    {
        Debug(importer.GetErrorString())
        return nullptr;
    }

    Mesh* mesh = new Mesh();

    ProcessNode(scene->mRootNode, scene, mesh);

    return mesh;
}

void MeshImporter::ProcessNode(aiNode* node, const aiScene* scene, Mesh* mesh)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];
        ProcessMesh(aiMesh, scene, mesh);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene, mesh);
    }
}

void MeshImporter::ProcessMesh(aiMesh* aiMesh, const aiScene* scene, Mesh* mesh)
{
    for (unsigned int i = 0; i < aiMesh->mNumVertices; i++)
    {
        Vertex vertex;
        vertex.position = Vec3(aiMesh->mVertices[i].x, aiMesh->mVertices[i].y, aiMesh->mVertices[i].z);
        vertex.normal = Vec3(aiMesh->mNormals[i].x, aiMesh->mNormals[i].y, aiMesh->mNormals[i].z);
        //todo assuming one UV/color channel for now
        if (aiMesh->HasTextureCoords(0))
        {
            vertex.uv = Vec2(aiMesh->mTextureCoords[0]->x, aiMesh->mTextureCoords[0]->y);
        }
        if (aiMesh->HasVertexColors(0))
        {
            vertex.color = Vec4(aiMesh->mColors[0]->r, aiMesh->mColors[0]->g, aiMesh->mColors[0]->b,
                                aiMesh->mColors[0]->a);
        }
        mesh->Vertices.push_back(vertex);
    }

    if (aiMesh->HasFaces())
    {
        for (unsigned int i = 0; i < aiMesh->mNumFaces; i++)
        {
            aiFace face = aiMesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
            {
                mesh->Indices.push_back(face.mIndices[j]);
            }
        }
    }
}
