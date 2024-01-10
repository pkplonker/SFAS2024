#include "IMaterial.h"

#include "ResourceManager.h"

IMaterial::IMaterial(Vec4 color) : materialProperties(MaterialProperties())
{
    SetShader(ResourceManager::GetShader());
    this->color = color;
}

IMaterial::~IMaterial()
{
}

MaterialProperties& IMaterial::GetMaterialProperties()
{
    materialProperties.UseTexture = texture != nullptr;
    return materialProperties;
}
