#include "ResourceManager.h"

#include <sstream>

#include "IGraphics.h"
#include "IShader.h"
#include "ITexture.h"
#include "MeshSerializer.h"
#include "IMaterial.h"

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

IShader* ResourceManager::GetShader(std::wstring shaderPath, std::string vsentry, std::string vsshader,
	std::string psentry, std::string psshader)
{
	const auto key = GenerateShaderKey(shaderPath, vsentry, vsshader, psentry, psshader);
	if (const auto it = shaders.find(key); it != shaders.end())
	{
		return it->second.get();
	}

	IShader* shader = graphics->CreateShader(shaderPath.c_str(), vsentry.c_str(), vsshader.c_str(), psentry.c_str(),
		psshader.c_str());
	shaders[key] = std::unique_ptr<IShader>(shader);
	return shader;
}

IMaterial* ResourceManager::GetMaterial(std::wstring shaderPath, std::wstring texturePath)
{
	const auto key = GenerateMaterialKey(shaderPath, texturePath);
	if (const auto it = materials.find(key); it != materials.end())
	{
		return it->second.get();
	}

	IMaterial* material = graphics->CreateMaterial(GetShader(shaderPath), GetTexture(texturePath));
	materials[key] = std::unique_ptr<IMaterial>(material);
	return material;
}

std::wstring ResourceManager::GenerateShaderKey(const std::wstring& shaderPath, const std::string& vsEntry,
	const std::string& vsShader, const std::string& psEntry, const std::string& psShader)
{
	std::wstringstream keyStream;

	const std::wstring wVsEntry(vsEntry.begin(), vsEntry.end());
	const std::wstring wVsShader(vsShader.begin(), vsShader.end());
	const std::wstring wPsEntry(psEntry.begin(), psEntry.end());
	const std::wstring wPsShader(psShader.begin(), psShader.end());

	keyStream << shaderPath
		<< wVsEntry
		<< wVsShader
		<< wPsEntry
		<< wPsShader;

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