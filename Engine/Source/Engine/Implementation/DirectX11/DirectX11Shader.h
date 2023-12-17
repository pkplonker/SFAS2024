
#pragma once

#include "Engine/IShader.h"
#include <d3d11.h>

class DirectX11Graphics;

class DirectX11Shader : public IShader
{
public:

	DirectX11Shader(std::wstring path, ID3D11DeviceContext* ContextIn, ID3D11VertexShader* VertexShaderIn,	ID3D11PixelShader* PixelShaderIn, ID3D11InputLayout* InputLayoutIn);
	virtual ~DirectX11Shader();
	virtual void Update();
	void Reload() override;
	void SetVertexShader(ID3D11VertexShader* vertexShader);
	void SetPixelShader(ID3D11PixelShader* pixelShader);
	void SetInputLayout(ID3D11InputLayout* inputLayout);
private:

	ID3D11DeviceContext* Context;
	ID3D11VertexShader* VertexShader;
	ID3D11PixelShader* PixelShader;
	ID3D11InputLayout* InputLayout;
};

