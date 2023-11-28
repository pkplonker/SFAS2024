#include "IMaterial.h"

IMaterial::IMaterial(Vec4 color)
{
    this->color = color;
}

IMaterial::~IMaterial()
{
}

void IMaterial::UpdateMaterialBuffer(MaterialBufferObject* data)
{
    data->color = color;
}