#include "DirectX11Shader.h"
#include "DirectX11Texture.h"
#include "IApplication.h"
#include "IGraphics.h"
#include "ResourceManager.h"
#include "Implementation/Logging/Debug.h"


DirectX11Shader::DirectX11Shader(std::wstring path, ID3D11DeviceContext* ContextIn, ID3D11VertexShader* VertexShaderIn,
                                 ID3D11PixelShader* PixelShaderIn, ID3D11InputLayout* InputLayoutIn) : IShader(path),
    Context(ContextIn), VertexShader(VertexShaderIn), PixelShader(PixelShaderIn), InputLayout(InputLayoutIn)
{
}

DirectX11Shader::~DirectX11Shader()
{
    if (InputLayout)
    {
        InputLayout->Release();
    }

    if (VertexShader)
    {
        VertexShader->Release();
    }

    if (PixelShader)
    {
        PixelShader->Release();
    }
}

void DirectX11Shader::Update()
{
    if (Context)
    {
        Context->IASetInputLayout(InputLayout);
        Context->VSSetShader(VertexShader, 0, 0);
        Context->PSSetShader(PixelShader, 0, 0);
    }
}

void DirectX11Shader::Reload()
{
    Trace("Reloading shader")
    ResourceManager::ReloadShader(this);
}

void DirectX11Shader::SetVertexShader(ID3D11VertexShader* vertexShader)
{
    this->VertexShader = vertexShader;
}

void DirectX11Shader::SetPixelShader(ID3D11PixelShader* pixelShader)
{
    this->PixelShader = pixelShader;
}

void DirectX11Shader::SetInputLayout(ID3D11InputLayout* inputLayout)
{
    this->InputLayout = inputLayout;
}
