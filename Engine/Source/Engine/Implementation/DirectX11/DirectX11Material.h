#pragma once
#include "IMaterial.h"

class ITexture;
class IShader;

class DirectX11Material : public IMaterial
{
public:
	DirectX11Material();
	DirectX11Material(IShader* shader);
	DirectX11Material(IShader* shader, ITexture* texture);
	bool Update() override;
	~DirectX11Material() override;
};
