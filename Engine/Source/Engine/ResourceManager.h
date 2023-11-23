#pragma once
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>

#include "IGraphics.h"
#include "IShader.h"
#include "ITexture.h"
#include "MeshSerializer.h"


const std::wstring DEFAULT_UNLIT_SHADER = L"Resource/Shaders/UnlitColor.fx";

const std::string DEFAULT_VS_SHADER = "vs_4_0";
const std::string DEFAULT_PS_SHADER = "ps_4_0";

const std::string DEFAULT_VS_ENRTY = "VS_Main";

const std::string DEFAULT_PS_ENRTY = "PS_Main";

class ResourceManager
{
public:
    ResourceManager(IGraphics* Graphics) : graphics(Graphics)
    {
    }

    Mesh* GetMesh(const std::string path)
    {
        auto it = meshes.find(path);
        if (it != meshes.end())
        {
            return it->second.get();
        }
        Mesh* mesh = MeshSerializer::Deserialize(path);
        meshes[path] = std::unique_ptr<Mesh>(mesh);
        return mesh;
    }
    
    ITexture* GetTexture(const std::wstring path)
    {
        auto it = textures.find(path);
        if (it != textures.end())
        {
            return it->second.get();
        }
        ITexture* texture = graphics->CreateTexture(path.c_str());
        textures[path] = std::unique_ptr<ITexture>(texture);
        return texture;
    }

    IShader* GetShader(ITexture* texture, std::wstring shaderPath = DEFAULT_UNLIT_SHADER,
                       std::string vsentry = DEFAULT_VS_ENRTY, std::string vsshader = DEFAULT_VS_SHADER,
                       std::string psentry = DEFAULT_PS_ENRTY, std::string psshader = DEFAULT_PS_SHADER)
    {
        auto key = GenerateShaderKey(shaderPath, vsentry, vsshader, psentry, psshader, texture);
        auto it = shaders.find(key);
        if (it != shaders.end())
        {
            return it->second.get();
        }

        IShader* shader = graphics->CreateShader(shaderPath.c_str(), vsentry.c_str(), vsshader.c_str(), psentry.c_str(),
                                                 psshader.c_str(), texture);
        shaders[key] = std::unique_ptr<IShader>(shader);
        return shader;
    }

    IShader* GetShader(const std::wstring texturePath, std::wstring shaderPath = DEFAULT_UNLIT_SHADER,
                       std::string vsentry = DEFAULT_VS_ENRTY, std::string vsshader = DEFAULT_VS_SHADER,
                       std::string psentry = DEFAULT_PS_ENRTY, std::string psshader = DEFAULT_PS_SHADER)
    {
        auto texture = GetTexture(texturePath);
        auto key = GenerateShaderKey(shaderPath, vsentry, vsshader, psentry, psshader, texture);
        auto it = shaders.find(key);
        if (it != shaders.end())
        {
            return it->second.get();
        }

        IShader* shader = graphics->CreateShader(shaderPath.c_str(), vsentry.c_str(), vsshader.c_str(), psentry.c_str(),
                                                 psshader.c_str(), texture);
        shaders[key] = std::unique_ptr<IShader>(shader);
        return shader;
    }

    std::wstring GenerateShaderKey(const std::wstring& shaderPath,
                                   const std::string& vsEntry,
                                   const std::string& vsShader,
                                   const std::string& psEntry,
                                   const std::string& psShader,
                                   ITexture* texture)
    {
        std::wstringstream keyStream;

        std::wstring wVsEntry(vsEntry.begin(), vsEntry.end());
        std::wstring wVsShader(vsShader.begin(), vsShader.end());
        std::wstring wPsEntry(psEntry.begin(), psEntry.end());
        std::wstring wPsShader(psShader.begin(), psShader.end());

        keyStream << shaderPath
            << wVsEntry
            << wVsShader
            << wPsEntry
            << wPsShader
            << static_cast<void*>(texture); //hopefully this works :/

        return keyStream.str();
    }

private:
    IGraphics* graphics;
    std::unordered_map<std::wstring, std::unique_ptr<ITexture>> textures;
    std::unordered_map<std::wstring, std::unique_ptr<IShader>> shaders;
    std::unordered_map<std::string, std::unique_ptr<Mesh>> meshes;

};
