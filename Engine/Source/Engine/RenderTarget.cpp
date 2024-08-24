#include "RenderTarget.h"

#include "Implementation/DirectX11/DirectX11Graphics.h"
#include "implementation/Logging/Debug.h"

IRenderTarget::IRenderTarget(DirectX11Graphics* dx11Graphics) : device(dx11Graphics->GetDevice()),
                                                                context(dx11Graphics->GetContext()),
                                                                swapChain(dx11Graphics->GetSwapChain())
{
}

void WindowRenderTarget::Resize(int newWidth, int newHeight)
{
    Width = newWidth;
    Height = newHeight;
    context->OMSetRenderTargets(0, nullptr, nullptr);
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

    HRESULT hr = swapChain->ResizeBuffers(0, newWidth, newHeight, DXGI_FORMAT_UNKNOWN, 0);
    if (FAILED(hr))
    {
        Error("Failed resizing swapchain buffersr")
    }

    ID3D11Texture2D* backBuffer = nullptr;
    hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
    if (SUCCEEDED(hr))
    {
        hr = device->CreateRenderTargetView(backBuffer, nullptr, &RenderTargetView);
        backBuffer->Release();
    }
    if (FAILED(hr))
    {
        Error("Failed resetting back buffer")
    }

    D3D11_TEXTURE2D_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
    depthStencilDesc.Width = newWidth;
    depthStencilDesc.Height = newHeight;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    ID3D11Texture2D* depthStencilBuffer;
    hr = device->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencilBuffer);
    if (SUCCEEDED(hr))
    {
        hr = device->CreateDepthStencilView(depthStencilBuffer, nullptr, &DepthStencilView);
        depthStencilBuffer->Release();
    }
    if (FAILED(hr))
    {
        Error("Failed modifying depth buffer")
    }

    context->OMSetRenderTargets(1, &RenderTargetView, DepthStencilView);

    D3D11_VIEWPORT viewport = {};
    viewport.Width = static_cast<float>(newWidth);
    viewport.Height = static_cast<float>(newHeight);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    context->RSSetViewports(1, &viewport);
}
