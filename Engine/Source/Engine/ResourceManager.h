#pragma once
#include <memory>
#include <string>
#include <unordered_map>

#include "IMaterial.h"
#include "IShader.h"
#include "ITexture.h"
#include "Implementation/Mesh.h"

class IGraphics;

const std::wstring DEFAULT_UNLIT_SHADER = L"Resource/Shaders/UnlitColor.fx";
const std::string DEFAULT_VS_SHADER = "vs_4_0";
const std::string DEFAULT_PS_SHADER = "ps_4_0";
const std::string DEFAULT_VS_ENRTY = "VS_Main";
const std::string DEFAULT_PS_ENRTY = "PS_Main";

class ResourceManager
{
public:
	ResourceManager(IGraphics* Graphics);

	Mesh* GetMesh(std::string path);

	ITexture* GetTexture(std::wstring path);

	IShader* GetShader(std::wstring shaderPath = DEFAULT_UNLIT_SHADER,
		std::string vsentry = DEFAULT_VS_ENRTY, std::string vsshader = DEFAULT_VS_SHADER,
		std::string psentry = DEFAULT_PS_ENRTY, std::string psshader = DEFAULT_PS_SHADER);

	IMaterial* GetMaterial(std::wstring shaderPath = DEFAULT_UNLIT_SHADER, std::wstring texturePath = L"");

	std::wstring GenerateShaderKey(const std::wstring& shaderPath,
		const std::string& vsEntry,
		const std::string& vsShader,
		const std::string& psEntry,
		const std::string& psShader);

	std::wstring GenerateMaterialKey(const std::wstring& shaderPath,
		const std::wstring& texturePath);

private:
	IGraphics* graphics;
	std::unordered_map<std::wstring, std::unique_ptr<ITexture>> textures;
	std::unordered_map<std::wstring, std::unique_ptr<IShader>> shaders;
	std::unordered_map<std::string, std::unique_ptr<Mesh>> meshes;
	std::unordered_map<std::wstring, std::unique_ptr<IMaterial>> materials;
};
