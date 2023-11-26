#include "IMaterial.h"

#include "IShader.h"
#include "ITexture.h"

IMaterial::~IMaterial()
{
    delete shader;
    delete texture;
}
