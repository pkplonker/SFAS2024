#pragma once
#include "Engine/Math/Vector4.h"
class ITexture;
class IShader;

const float MATERIAL_DEFAULT_COLOR = 1.0f;

struct MaterialBufferObject {
    Vec4 color;
    bool useTex = false;
    float padding[3]{0,0,0};
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
    void UpdateMaterialBuffer(MaterialBufferObject* data);

protected:
    IShader* shader = {};
    ITexture* texture = {};
    Vec4 color = MATERIAL_DEFAULT_COLOR;
};
