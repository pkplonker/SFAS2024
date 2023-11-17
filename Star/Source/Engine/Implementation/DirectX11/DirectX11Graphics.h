#pragma once

#include <d3d11.h>

#include "Engine/IGraphics.h"
#include "Engine/Transform2D.h"
#include <Windows.h>
#include <d3dcommon.h>
#include <DirectXMath.h>
#include <memory>

#include "Engine/ICamera.h"

struct Transform3D;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11Texture2D;
struct ID3D11Buffer;
struct ID3D11BlendState;
enum D3D_FEATURE_LEVEL;

class DirectX11Graphics : public IGraphics
{
public:

	DirectX11Graphics(HWND hwndIn);
	~DirectX11Graphics() override;

	void Update() override;
	void PostUpdate() override;
	bool IsValid() override;

	ITexture* CreateTexture(const wchar_t* filepath) override;
	IShader* CreateShader(const wchar_t* filepath, const char* vsentry, const char* vsshader, const char* psentry, const char* psshader, ITexture* TextureIn) override;
	std::shared_ptr<IRenderable> CreateBillboard(IShader* ShaderIn) override;
	std::shared_ptr<IRenderable> CreateMeshRenderable(IShader* ShaderIn) override;
	void SetActiveCamera(std::shared_ptr<ICamera> camera) override;
	ID3D11Device* GetDevice() const { return Device; }
	HWND GetHWND() const { return hwnd; }
	ID3D11DeviceContext* GetContext() const { return Context; }

	int GetWidth()override {return width;}
	int GetHeight()override {return height;}

protected:

	virtual void SetWorldMatrix(std::weak_ptr<Transform3D> transform);
	virtual bool CompileShader(LPCWSTR filepath, LPCSTR entry, LPCSTR shader, ID3DBlob** buffer);

private:

	ID3D11Device* Device;
	ID3D11DeviceContext* Context;
	IDXGISwapChain* SwapChain;
	ID3D11RenderTargetView* BackbufferView;
	ID3D11Texture2D* BackbufferTexture;
	ID3D11DepthStencilView* DepthStencilView;
	ID3D11BlendState* BlendState;
	ID3D11Buffer* Mvp;
	DirectX::XMMATRIX vpMatrix;
	D3D_FEATURE_LEVEL FeatureLevel;
	HWND hwnd;
	int width;
	int height;
	std::shared_ptr<ICamera> camera;

};

