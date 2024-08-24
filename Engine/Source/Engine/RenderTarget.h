#pragma once
#include "IMeshRenderable.h"

class DirectX11Graphics;
class Scene;

class IRenderTarget
{
public:
    virtual ~IRenderTarget()
    {
        if (RenderTargetView != nullptr)
        {
            RenderTargetView->Release();
            RenderTargetView = nullptr;
        }
        if (DepthStencilView != nullptr)
        {
            DepthStencilView->Release();
            DepthStencilView = nullptr;
        }
        if (BackbufferTexture != nullptr)
        {
            BackbufferTexture->Release();
            BackbufferTexture = nullptr;
        }
    }

    IRenderTarget(DirectX11Graphics* dx11Graphics);

    ID3D11RenderTargetView* RenderTargetView = nullptr;
    ID3D11DepthStencilView* DepthStencilView = nullptr;

    ID3D11Texture2D* BackbufferTexture = nullptr;

    virtual void Resize(int newWidth, int newHeight) =0;
    virtual void Render(Scene* scene) =0;
    int Width = 1;
    int Height = 1;

protected:
    ID3D11Device* device = nullptr;
    ID3D11DeviceContext* context = nullptr;
    IDXGISwapChain* swapChain = nullptr;
};

class WindowRenderTarget : public IRenderTarget
{
public:
    WindowRenderTarget(DirectX11Graphics* dx11Graphics, int width, int height) : IRenderTarget(dx11Graphics)
    {
        WindowRenderTarget::Resize(width, height);
    }


    ID3D11Texture2D* BackbufferTexture;

    void Resize(int newWidth, int newHeight) override;

    void Render(Scene* scene) override
    {
    }
};

class TextureRenderTarget : public IRenderTarget
{
public:
    TextureRenderTarget(DirectX11Graphics* dx11Graphics, int width, int height) : IRenderTarget(dx11Graphics)
    {
    }

    void Resize(int newWidth, int newHeight) override
    {
    }

    void Render(Scene* scene) override
    {
    }
};
