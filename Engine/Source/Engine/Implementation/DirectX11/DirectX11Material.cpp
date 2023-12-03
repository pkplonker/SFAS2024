#include "DirectX11Material.h"

#include "IApplication.h"
#include "IGraphics.h"
#include "IShader.h"
#include "ITexture.h"
#include "ResourceManager.h"

DirectX11Material::DirectX11Material() 
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
