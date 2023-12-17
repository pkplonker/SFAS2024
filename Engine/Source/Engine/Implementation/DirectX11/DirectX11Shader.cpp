#include "DirectX11Shader.h"
#include "DirectX11Texture.h"
#include "IApplication.h"


DirectX11Shader::DirectX11Shader(std::wstring path, ID3D11DeviceContext* ContextIn, ID3D11VertexShader* VertexShaderIn, ID3D11PixelShader* PixelShaderIn, ID3D11InputLayout* InputLayoutIn) : IShader(path), Context(ContextIn), VertexShader(VertexShaderIn), PixelShader(PixelShaderIn), InputLayout(InputLayoutIn) 
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
	// IApplication::GetGraphics()->
}
