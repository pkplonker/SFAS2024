#pragma once
#include "Engine/Math/Vector4.h"
#include "Implementation/DirectX11/DirectX11Graphics.h"
class ITexture;
class IShader;

const float MATERIAL_DEFAULT_COLOR = 1.0f;

struct MaterialProperties
{
    MaterialProperties()
        : Emissive(0.2f, 0.2f, 0.2f, 1.0f)
          , Ambient(0.1f, 0.1f, 0.1f, 1.0f)
          , Diffuse(1.0f, 1.0f, 1.0f, 1.0f)
          , Specular(1.0f, 1.0f, 1.0f, 1.0f)
          , SpecularPower(32.0f)
          , UseTexture(false)
          , Color(1.0f, 1.0f, 1.0f, 1.0f)
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
    virtual Vec4 GetColor() { return color; }
    virtual void SetColor(Vec4 value) { color = value; }
    virtual bool GetIsSkybox() =0;
    virtual void SetIsSkyBox(bool val) =0;
    MaterialProperties GetMaterialProperties();

protected:
    IShader* shader = {};
    ITexture* texture = {};
    Vec4 color = MATERIAL_DEFAULT_COLOR;
    MaterialProperties materialProperties;
};
