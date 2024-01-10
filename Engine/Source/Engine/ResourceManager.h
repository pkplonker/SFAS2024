#pragma once
#include <memory>
#include <string>
#include <unordered_map>

#include "IMaterial.h"
#include "IShader.h"
#include "ITexture.h"
#include "Implementation/Mesh.h"
#include "Implementation/DirectX11/DirectX11Graphics.h"

class IGraphics;

const std::wstring DEFAULT_UNLIT_SHADER =
    L"S:/Users/pkplo/OneDrive/Documents/C++/SFAS2024/Editor/Resource/Shaders/boing.fx";
const std::string DEFAULT_VS_SHADER = "vs_4_0";
const std::string DEFAULT_PS_SHADER = "ps_4_0";
const std::string DEFAULT_VS_ENRTY = "VS_Main";
const std::string DEFAULT_PS_ENRTY = "PS_Main";

class ResourceManager
{
public:
    static Mesh* GetMesh(std::string path);

    static ITexture* GetTexture(std::wstring path);

    static IShader* GetShader(const std::wstring& shaderPath = DEFAULT_UNLIT_SHADER,
                              const std::string& vsentry = DEFAULT_VS_ENRTY,
                              const std::string& vsshader = DEFAULT_VS_SHADER,
                              const std::string& psentry = DEFAULT_PS_ENRTY,
                              const std::string& psshader = DEFAULT_PS_SHADER);

    static IMaterial* GetMaterial(std::wstring shaderPath = DEFAULT_UNLIT_SHADER, std::wstring texturePath = L"");

    static std::wstring GenerateShaderKey(const std::wstring& shaderPath);

    static std::wstring GenerateMaterialKey(const std::wstring& shaderPath,
                                            const std::wstring& texturePath);
    static void Init(IGraphics* graphics);
    static void ReloadShader(IShader* shader);

    static std::vector<IShader*> GetShaders()
    {
        std::vector<IShader*> shaderVect;
        shaderVect.reserve(shaders.size());

        for (const auto& shaderPair : shaders)
        {
            IShader* shaderPtr = shaderPair.second.get();
            if (shaderPtr != nullptr)
            {
                shaderVect.emplace_back(shaderPtr);
            }
        }

        return shaderVect;
    }

private:
    inline static IGraphics* graphics;
    inline static std::unordered_map<std::wstring, std::unique_ptr<ITexture>> textures;
    inline static std::unordered_map<std::wstring, std::unique_ptr<IShader>> shaders;
    inline static std::unordered_map<std::string, std::unique_ptr<Mesh>> meshes;
    inline static std::unordered_map<std::wstring, std::unique_ptr<IMaterial>> materials;
};
