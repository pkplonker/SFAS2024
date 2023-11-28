#include "DirectX11Material.h"

#include "IShader.h"
#include "ITexture.h"

DirectX11Material::DirectX11Material() : DirectX11Material(nullptr, nullptr)
{
}

DirectX11Material::DirectX11Material(IShader* shader) : DirectX11Material(shader, nullptr)
{
}

DirectX11Material::DirectX11Material(IShader* shader, ITexture* texture) : IMaterial()
{
    SetShader(shader);
    SetTexture(texture);
}

void DirectX11Material::Update()
{
    if (shader) shader->Update();
    if (texture) texture->Update();
}

DirectX11Material::~DirectX11Material()
{
}
