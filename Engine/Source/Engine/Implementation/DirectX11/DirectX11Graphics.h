#pragma once

#include <d3d11.h>

#include "Engine/IGraphics.h"
#include <Windows.h>
#include <d3dcommon.h>
#include <DirectXMath.h>
#include <memory>

#include "RenderingStats.h"
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

	void SetDirectionalLightBuffers();
	void Update() override;
	void UpdateRenderable(IMaterial* mat, const std::shared_ptr<IRenderable>& renderable) override;
	void RemoveRenderable(const std::shared_ptr<IRenderable>& renderable) override;
	void PostUpdate() override;
	bool IsValid() override;
	void RenderBucket(RenderingStats& stats, IShader* previousShader, std::map<IMaterial*, std::list<std::shared_ptr<IRenderable>>>::iterator bucket);

	ITexture* CreateTexture(const wchar_t* filepath) override;
	bool TryCreateShaderData(const wchar_t* filepath, const char* vsentry, const char* vsshader, const char* psentry,
	                         const char* psshader, ID3D11VertexShader** VertexShader, ID3D11PixelShader** PixelShader,
	                         ID3D11InputLayout** InputLayout);
	IShader* CreateShader(const wchar_t* filepath, const char* vsentry, const char* vsshader, const char* psentry, const char* psshader) override;
	std::shared_ptr<IRenderable> CreateBillboard(IMaterial* material) override;
	void AddRenderable(IMaterial* material, std::shared_ptr<IRenderable> Result);
	std::shared_ptr<IMeshRenderable> CreateMeshRenderable(IMaterial* material, Mesh* mesh) override;
	std::shared_ptr<IMeshRenderable> CreateMeshRenderable(Mesh* mesh) override;
	void SetActiveCamera(std::shared_ptr<ICamera> camera) override;
	ID3D11Device* GetDevice() const { return Device; }
	HWND GetHWND() const { return hwnd; }
	ID3D11DeviceContext* GetContext() const { return Context; }
	void SetRenderToTexture(bool state, int width, int height) override;
	int GetWidth()override { return width; }
	int GetHeight()override { return height; }
	ID3D11ShaderResourceView* GetTextureView() const;
	ID3D11Texture2D* GetTexture() const;
	int GetTextureWidth() { return texWidth; }
	int GetTextureHeight() { return texHeight; }
	IMaterial* CreateMaterial(IShader* shader,ITexture* texture) override;
	void Resize(int width, int height) override;
	void UpdateRenderToTextureResources(int newWidth, int newHeight);
	bool TryUpdateShader(IShader* shader, const char* vsentry, const char* vsshader, const char* psentry,
	                     const char* psshader) override;
protected:

	virtual void SetMatrixBuffers(std::weak_ptr<Transform3D> transform);
	virtual bool CompileShader(LPCWSTR filepath, LPCSTR entry, LPCSTR shader, ID3DBlob** buffer);


private:

	ID3D11Device* Device = nullptr;
	ID3D11DeviceContext* Context = nullptr;
	IDXGISwapChain* SwapChain = nullptr;
	ID3D11RenderTargetView* BackbufferView = nullptr;
	ID3D11Texture2D* BackbufferTexture = nullptr;
	ID3D11DepthStencilView* DepthStencilView = nullptr;
	ID3D11BlendState* BlendState = nullptr;
	ID3D11Buffer* Mvp = nullptr;
	DirectX::XMMATRIX vpMatrix;
	D3D_FEATURE_LEVEL FeatureLevel;
	HWND hwnd;
	int width;
	int height;
	int texWidth;
	int texHeight;
	std::shared_ptr<ICamera> camera;
	ID3D11Texture2D* renderTargetTexture = nullptr;
	ID3D11RenderTargetView* renderTargetView = nullptr;
	ID3D11ShaderResourceView* shaderResourceView = nullptr;
	ID3D11Texture2D* textureTargetDepthStencilBuffer = nullptr;
	ID3D11DepthStencilView* textureTargetDepthStencilView = nullptr;
	bool renderToTexture;
	ID3D11Buffer* materialBuffer;
	D3D11_BUFFER_DESC materialBufferDesc;
	ID3D11Buffer* directionalLightBuffer;
	D3D11_BUFFER_DESC directionalLightBufferDesc;
	ID3D11DepthStencilState* depthState;
	ID3D11DepthStencilState* skyDepthState;
};
