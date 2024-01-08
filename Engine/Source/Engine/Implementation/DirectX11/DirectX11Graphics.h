#pragma once

#include <d3d11.h>

#include "Engine/IGraphics.h"
#include <Windows.h>
#include <d3dcommon.h>
#include <DirectXMath.h>
#include <memory>

#include "RenderingStats.h"
#include "Engine/ICamera.h"
#include "Engine/Math/Vector4.h"

struct Vec4;
struct Transform3D;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11Texture2D;
struct ID3D11Buffer;
struct ID3D11BlendState;
enum D3D_FEATURE_LEVEL;
#define MAX_LIGHTS 8

struct _Material
{
    _Material()
        : Emissive(0.0f, 0.0f, 0.0f, 1.0f)
          , Ambient(0.1f, 0.1f, 0.1f, 1.0f)
          , Diffuse(1.0f, 1.0f, 1.0f, 1.0f)
          , Specular(1.0f, 1.0f, 1.0f, 1.0f)
          , SpecularPower(128.0f)
          , UseTexture(false)
    {
    }

    DirectX::XMFLOAT4 Emissive;
    //----------------------------------- (16 byte boundary)
    DirectX::XMFLOAT4 Ambient;
    //----------------------------------- (16 byte boundary)
    DirectX::XMFLOAT4 Diffuse;
    //----------------------------------- (16 byte boundary)
    DirectX::XMFLOAT4 Specular;
    //----------------------------------- (16 byte boundary)
    float SpecularPower;
    // Add some padding complete the 16 byte boundary.
    int UseTexture;
    // Add some padding to complete the 16 byte boundary.
    float Padding[2];
    //----------------------------------- (16 byte boundary)
}; // Total:                                80 bytes (5 * 16)

struct MaterialProperties
{
    _Material Material;
};

enum LightType
{
    DirectionalLight = 0,
    PointLight = 1,
    SpotLight = 2
};

struct Light
{
    Light()
        : Position(0.0f, 0.0f, 0.0f, 1.0f)
          , Direction(0.0f, 0.0f, 1.0f, 0.0f)
          , Color(1.0f, 1.0f, 1.0f, 1.0f)
          , SpotAngle(DirectX::XM_PIDIV2)
          , ConstantAttenuation(1.0f)
          , LinearAttenuation(0.0f)
          , QuadraticAttenuation(0.0f)
          , LightType(DirectionalLight)
          , Enabled(0)
    {
    }

    DirectX::XMFLOAT4 Position;
    //----------------------------------- (16 byte boundary)
    DirectX::XMFLOAT4 Direction;
    //----------------------------------- (16 byte boundary)
    DirectX::XMFLOAT4 Color;
    //----------------------------------- (16 byte boundary)
    float SpotAngle;
    float ConstantAttenuation;
    float LinearAttenuation;
    float QuadraticAttenuation;
    //----------------------------------- (16 byte boundary)
    int LightType;
    int Enabled;
    // Add some padding to make this struct size a multiple of 16 bytes.
    int Padding[2];
    //----------------------------------- (16 byte boundary)
}; // Total:                              80 bytes ( 5 * 16 )

struct LightProperties
{
    LightProperties()
        : EyePosition(0.0f, 0.0f, 0.0f, 1.0f)
          , GlobalAmbient(0.2f, 0.2f, 0.8f, 1.0f)
    {
    }

    Vec4 EyePosition;
    //----------------------------------- (16 byte boundary)
    Vec4 GlobalAmbient;
    //----------------------------------- (16 byte boundary)
    Light Lights[MAX_LIGHTS]; // 80 * 8 bytes
}; // Total:                                  672 bytes (42 * 16)

class DirectX11Graphics : public IGraphics
{
public:
    DirectX11Graphics(HWND hwndIn);
    ~DirectX11Graphics() override;

    void Update() override;
    void UpdateRenderable(IMaterial* mat, const std::shared_ptr<IRenderable>& renderable) override;
    void RemoveRenderable(const std::shared_ptr<IRenderable>& renderable) override;
    void PostUpdate() override;
    bool IsValid() override;
    void RenderBucket(RenderingStats& stats, IShader* previousShader,
                      std::map<IMaterial*, std::list<std::shared_ptr<IRenderable>>>::iterator bucket);

    ITexture* CreateTexture(const wchar_t* filepath) override;
    bool TryCreateShaderData(const wchar_t* filepath, const char* vsentry, const char* vsshader, const char* psentry,
                             const char* psshader, ID3D11VertexShader** VertexShader, ID3D11PixelShader** PixelShader,
                             ID3D11InputLayout** InputLayout);
    IShader* CreateShader(const wchar_t* filepath, const char* vsentry, const char* vsshader, const char* psentry,
                          const char* psshader) override;
    std::shared_ptr<IRenderable> CreateBillboard(IMaterial* material) override;
    void AddRenderable(IMaterial* material, std::shared_ptr<IRenderable> Result);
    std::shared_ptr<IMeshRenderable> CreateMeshRenderable(IMaterial* material, Mesh* mesh) override;
    std::shared_ptr<IMeshRenderable> CreateMeshRenderable(Mesh* mesh) override;
    void SetActiveCamera(std::shared_ptr<ICamera> camera) override;
    ID3D11Device* GetDevice() const { return Device; }
    HWND GetHWND() const { return hwnd; }
    ID3D11DeviceContext* GetContext() const { return Context; }
    void SetRenderToTexture(bool state, int width, int height) override;
    int GetWidth() override { return width; }
    int GetHeight() override { return height; }
    ID3D11ShaderResourceView* GetTextureView() const;
    ID3D11Texture2D* GetTexture() const;
    int GetTextureWidth() { return texWidth; }
    int GetTextureHeight() { return texHeight; }
    IMaterial* CreateMaterial(IShader* shader, ITexture* texture) override;
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
    ID3D11DepthStencilState* depthState;
    ID3D11DepthStencilState* skyDepthState;
    ID3D11Buffer* perFrameConstantBuffer;
    ID3D11Buffer* perObjectConstantBuffer;
    ID3D11Buffer* lightPropertiesConstantBuffer;
    ID3D11Buffer* materialPropertiesConstantBuffer;
    LightProperties lightProperties;
};
