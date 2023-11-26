#pragma once

class ITexture;
class IShader;

class IMaterial
{
public:
    virtual ~IMaterial();
    virtual void Update() = 0;
    virtual void SetShader(IShader* value) { this->shader = value; }
    virtual void SetTexture(ITexture* value) { this->texture = value; }
    virtual IShader* GetShader() { return shader; }
    virtual ITexture* GetTexture() { return texture; }

protected:
    IShader* shader= {};
    ITexture* texture= {};
};
