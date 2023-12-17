#include "ResourceManager.h"

#include <sstream>

#include "IGraphics.h"
#include "IShader.h"
#include "ITexture.h"
#include "MeshSerializer.h"
#include "IMaterial.h"
#include "implementation/Helpers.h"
#include "Implementation/Logging/Debug.h"

Mesh* ResourceManager::GetMesh(const std::string path)
{
    if (const auto it = meshes.find(path); it != meshes.end())
    {
        return it->second.get();
    }
    Mesh* mesh = MeshSerializer::Deserialize(path);
    meshes[path] = std::unique_ptr<Mesh>(mesh);
    return mesh;
}

ITexture* ResourceManager::GetTexture(const std::wstring path)
{
    if (const auto it = textures.find(path); it != textures.end())
    {
        return it->second.get();
    }
    ITexture* texture = graphics->CreateTexture(path.c_str());
    textures[path] = std::unique_ptr<ITexture>(texture);
    return texture;
}

IShader* ResourceManager::GetShader(const std::wstring& shaderPath, const std::string& vsentry,
                                    const std::string& vsshader,
                                    const std::string& psentry, const std::string& psshader)
{
    auto key = GenerateShaderKey(shaderPath);

    Info("Shader key generated" + Helpers::WideStringToString(key))
    if (const auto it = shaders.find(key); it != shaders.end())
    {
        return it->second.get();
    }

    IShader* shader = graphics->CreateShader(key.c_str(), vsentry.c_str(), vsshader.c_str(), psentry.c_str(),
                                             psshader.c_str());
    shaders[key] = std::unique_ptr<IShader>(shader);
    return shader;
}

void ResourceManager::ReloadShader(IShader* shader)
{
    if(!graphics->TryUpdateShader(shader,DEFAULT_VS_ENRTY.c_str(), DEFAULT_VS_SHADER.c_str(), DEFAULT_PS_ENRTY.c_str(),
                              DEFAULT_PS_SHADER.c_str() ))
    {
        Warning("Failed to update shader")
    }
}


IMaterial* ResourceManager::GetMaterial(std::wstring shaderPath, std::wstring texturePath)
{
    //removing this for now to allow for individual materials

    // const auto key = GenerateMaterialKey(shaderPath, texturePath);
    // if (const auto it = materials.find(key); it != materials.end())
    // {
    // 	return it->second.get();
    // }

    IMaterial* material = graphics->CreateMaterial(GetShader(shaderPath), GetTexture(texturePath));
    //materials[key] = std::unique_ptr<IMaterial>(material);
    return material;
}

std::wstring ResourceManager::GenerateShaderKey(const std::wstring& shaderPath)
{
	std::wstringstream keyStream;
    
	keyStream << shaderPath;

	return keyStream.str();
}

std::wstring ResourceManager::GenerateMaterialKey(const std::wstring& shaderPath, const std::wstring& texturePath)
{
    std::wstringstream keyStream;

    keyStream << shaderPath << texturePath;

    return keyStream.str();
}

void ResourceManager::Init(IGraphics* gfx)
{
    graphics = gfx;
}
