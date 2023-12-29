#include "MeshImporter.h"

#include "Engine/Implementation/Logging/Debug.h"
#include "Engine/Implementation/Mesh.h"
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Importer.hpp"


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
        Warning(importer.GetErrorString())
        return nullptr;
    }

    Mesh* mesh = new Mesh(path);

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
    Vec3 minPoint(FLT_MAX, FLT_MAX, FLT_MAX);
    Vec3 maxPoint(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    for (unsigned int i = 0; i < aiMesh->mNumVertices; i++)
    {
        Vertex vertex;
        vertex.position = Vec3(aiMesh->mVertices[i].x, aiMesh->mVertices[i].y, aiMesh->mVertices[i].z);

        minPoint.X(min(minPoint.X(), vertex.position.X()));
        minPoint.Y(min(minPoint.Y(), vertex.position.Y()));
        minPoint.Z(min(minPoint.Z(), vertex.position.Z()));
        
        maxPoint.X(max(maxPoint.X(), vertex.position.X()));
        maxPoint.Y(max(maxPoint.Y(), vertex.position.Y()));
        maxPoint.Z(max(maxPoint.Z(), vertex.position.Z()));
        
        vertex.position = Vec3(aiMesh->mVertices[i].x, aiMesh->mVertices[i].y, aiMesh->mVertices[i].z);
        vertex.normal = Vec3(aiMesh->mNormals[i].x, aiMesh->mNormals[i].y, aiMesh->mNormals[i].z);
        //todo assuming one UV/color channel for now
        if (aiMesh->HasTextureCoords(0))
        {
            vertex.uv = Vec2(aiMesh->mTextureCoords[0][i].x, 1-aiMesh->mTextureCoords[0][i].y);
        }
        if (aiMesh->HasVertexColors(0))
        {
            vertex.color = Vec4(aiMesh->mColors[0]->r, aiMesh->mColors[0]->g, aiMesh->mColors[0]->b,
                                aiMesh->mColors[0]->a);
        }
        else
        {
            vertex.color = Vec4(MESH_DEFAULT_COLOR);
        }
        mesh->GetVerts().push_back(vertex);
    }
    Vec3 extents = (maxPoint - minPoint) * 0.5f;
    mesh->GetAABB().Extents = extents;
    if (aiMesh->HasFaces())
    {
        for (unsigned int i = 0; i < aiMesh->mNumFaces; i++)
        {
            aiFace face = aiMesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
            {
                mesh->GetIndices().push_back(face.mIndices[j]);
            }
        }
    }
}
