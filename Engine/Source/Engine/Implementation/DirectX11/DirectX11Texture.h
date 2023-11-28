
#pragma once

#include <d3d11.h>

#include "ITexture.h"

class DirectX11Texture : public ITexture
{
public:

	DirectX11Texture(std::wstring path, ID3D11DeviceContext* ContextIn, ID3D11ShaderResourceView* TextureIn, ID3D11SamplerState* SamplerIn, const D3D11_TEXTURE2D_DESC& DescriptionIn);
	~DirectX11Texture() override;

	void Update() override;
	float GetWidth() const override;
	float GetHeight() const override;

private:

	ID3D11DeviceContext* Context;
	ID3D11ShaderResourceView* Texture;
	ID3D11SamplerState* Sampler;
	D3D11_TEXTURE2D_DESC Description;
};

