#pragma once
#include "Engine/Math/Vector4.h"
#include "Implementation/DirectX11/DirectX11Graphics.h"
class ITexture;
class IShader;

const float MATERIAL_DEFAULT_COLOR = 1.0f;

struct MaterialProperties
{
    MaterialProperties()
        : Emissive(1.0f, 1.0f, 1.0f, 1.0f)
          , Ambient(1.0f, 1.0f, 1.0f, 1.0f)
          , Diffuse(1.0f, 1.0f, 1.0f, 1.0f)
          , Specular(1.0f, 1.0f, 1.0f, 1.0f)
          , SpecularPower(32.0f)
          , UseTexture(false)
          , Color(MATERIAL_DEFAULT_COLOR,MATERIAL_DEFAULT_COLOR,MATERIAL_DEFAULT_COLOR,MATERIAL_DEFAULT_COLOR)
    {
    }

    Vec4 Emissive;
    Vec4 Ambient;
    Vec4 Diffuse;
    Vec4 Specular;
    float SpecularPower;
    int UseTexture;
    float Padding[2];
    Vec4 Color;
};


class IMaterial
{
public:
    IMaterial(Vec4 color = MATERIAL_DEFAULT_COLOR);
    virtual ~IMaterial();
    virtual bool Update() = 0;
    virtual void SetShader(IShader* value) { this->shader = value; }
    virtual void SetTexture(ITexture* value) { this->texture = value; }
    virtual IShader* GetShader() { return shader; }
    virtual ITexture* GetTexture() { return texture; }
    MaterialProperties& GetMaterialProperties();

protected:
    IShader* shader = {};
    ITexture* texture = {};
    Vec4 color = MATERIAL_DEFAULT_COLOR;
    MaterialProperties materialProperties;
};
