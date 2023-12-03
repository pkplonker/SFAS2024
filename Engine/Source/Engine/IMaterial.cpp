#include "IMaterial.h"

#include "ResourceManager.h"

IMaterial::IMaterial(Vec4 color)
{
    SetShader(ResourceManager::GetShader());
    this->color = color;
}

IMaterial::~IMaterial()
{
}

void IMaterial::UpdateMaterialBuffer(MaterialBufferObject* data)
{
    data->color = color;
    data->useTex = texture != nullptr;
}