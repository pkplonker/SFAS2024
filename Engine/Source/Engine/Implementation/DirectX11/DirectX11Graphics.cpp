#include "DirectX11Graphics.h"

#include <algorithm>

#include "DirectX11Billboard.h"
#include "DirectX11Shader.h"
#include "DirectX11Texture.h"
#include <d3dcommon.h>
#include <d3d11.h>
#include <D3DCompiler.h>
#include <DDSTextureLoader.h>
#include <memory>

#include "DirectionalLightComponent.h"
#include "DirectX11Material.h"
#include "DirectX11Mesh.h"
#include "RenderingStats.h"
#include "RenderTarget.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Engine/Implementation/Logging/Debug.h"
#include "Implementation/Mesh.h"
#include "Implementation/Scene.h"
#include "Implementation/Vertex.h"
using namespace DirectX;

struct PerFrameConstantBufferData
{
    XMMATRIX ViewProjectionMatrix;
};

struct PerObjectConstantBufferData
{
    XMMATRIX WorldMatrix;
    XMMATRIX InverseTransposeWorldMatrix;
    XMMATRIX WorldViewProjectionMatrix;
};

#define MAX_LIGHTS 8

DirectX11Graphics::DirectX11Graphics(HWND hwndIn) : FeatureLevel(D3D_FEATURE_LEVEL_11_0), hwnd(hwndIn)
{
    RECT dimensions;
    GetClientRect(hwnd, &dimensions);
    auto width = dimensions.right - dimensions.left;
    auto height = dimensions.bottom - dimensions.top;
    

    D3D_DRIVER_TYPE driverTypes[] = {D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP, D3D_DRIVER_TYPE_SOFTWARE};
    unsigned int totalDriverTypes = ARRAYSIZE(driverTypes);

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    HRESULT hr = S_OK;
    unsigned int creationFlags = 0;

#ifdef _DEBUG
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    for (unsigned int count = 0; count < totalDriverTypes; ++count)
    {
        hr = D3D11CreateDeviceAndSwapChain(nullptr, driverTypes[count], nullptr, creationFlags, nullptr, 0,
                                           D3D11_SDK_VERSION,
                                           &sd, &SwapChain, &Device, &FeatureLevel, &Context);

        if (SUCCEEDED(hr))
        {
            break;
        }
    }
    
    windowRenderTarget = std::make_shared<WindowRenderTarget>(this,width,height);
    RenderTargets.emplace_back(windowRenderTarget);
    
    if (SUCCEEDED(hr))
    {
        hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&windowRenderTarget->BackbufferTexture));
    }

    if (SUCCEEDED(hr))
    {
        hr = Device->CreateRenderTargetView(windowRenderTarget->BackbufferTexture, 0, &windowRenderTarget->RenderTargetView);
    }

    if (windowRenderTarget->BackbufferTexture)
    {
        windowRenderTarget->BackbufferTexture->Release();
    }

    if (FAILED(hr))
    {
        MessageBox(nullptr, "Graphics Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
    }
    else
    {
        D3D11_BUFFER_DESC constantBufferDesc;
        ZeroMemory(&constantBufferDesc, sizeof(D3D11_BUFFER_DESC));
        constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        constantBufferDesc.ByteWidth = sizeof(PerFrameConstantBufferData);
        constantBufferDesc.CPUAccessFlags = 0;
        constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

        hr = Device->CreateBuffer(&constantBufferDesc, nullptr, &perFrameConstantBuffer);
        if (FAILED(hr))
        {
            MessageBoxA(nullptr, "Failed to create constant buffer for per-frame data.", "Error", MB_OK | MB_ICONERROR);
            return;
        }
        constantBufferDesc.ByteWidth = sizeof(PerObjectConstantBufferData);

        hr = Device->CreateBuffer(&constantBufferDesc, nullptr, &perObjectConstantBuffer);
        if (FAILED(hr))
        {
            MessageBoxA(nullptr, "Failed to create constant buffer for per-object data.", "Error",
                        MB_OK | MB_ICONERROR);
            return;
        }
        constantBufferDesc.ByteWidth = sizeof(MaterialProperties);

        hr = Device->CreateBuffer(&constantBufferDesc, nullptr, &materialPropertiesConstantBuffer);
        if (FAILED(hr))
        {
            MessageBoxA(nullptr, "Failed to create constant buffer for material properties.", "Error",
                        MB_OK | MB_ICONERROR);
            return;
        }

        constantBufferDesc.ByteWidth = sizeof(LightProperties);
        hr = Device->CreateBuffer(&constantBufferDesc, nullptr, &lightPropertiesConstantBuffer);
        if (FAILED(hr))
        {
            MessageBoxA(nullptr, "Failed to create constant buffer for light properties.", "Error",
                        MB_OK | MB_ICONERROR);
            return;
        }

        D3D11_BLEND_DESC Desc;
        ZeroMemory(&Desc, sizeof(D3D11_BLEND_DESC));
        Desc.RenderTarget[0].BlendEnable = TRUE;
        Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
        Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
        Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        Device->CreateBlendState(&Desc, &BlendState);

        D3D11_DEPTH_STENCIL_DESC depthStencildesc = {};
        depthStencildesc.DepthEnable = true;
        depthStencildesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        depthStencildesc.DepthFunc = D3D11_COMPARISON_LESS;
        Device->CreateDepthStencilState(&depthStencildesc, &depthState);

        depthStencildesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        depthStencildesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
        Device->CreateDepthStencilState(&depthStencildesc, &skyDepthState);

        Context->OMSetDepthStencilState(depthState, 1u);

        ID3D11Texture2D* depthStencil;
        D3D11_TEXTURE2D_DESC depthDesc = {};
        depthDesc.Width = width;
        depthDesc.Height = height;
        depthDesc.MipLevels = 1;
        depthDesc.ArraySize = 1;
        depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
        depthDesc.SampleDesc.Count = 1;
        depthDesc.SampleDesc.Quality = 0;
        depthDesc.Usage = D3D11_USAGE_DEFAULT;
        depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        Device->CreateTexture2D(&depthDesc, nullptr, &depthStencil);

        D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc = {};
        depthViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
        depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        depthViewDesc.Texture2D.MipSlice = 0;
        Device->CreateDepthStencilView(depthStencil, &depthViewDesc, &windowRenderTarget->DepthStencilView);

        Context->OMSetRenderTargets(1, &windowRenderTarget->RenderTargetView, windowRenderTarget->DepthStencilView);
    }
}


DirectX11Graphics::~DirectX11Graphics()
{
    if (BlendState)
    {
        BlendState->Release();
    }
    
    if (SwapChain)
    {
        SwapChain->Release();
    }

    if (Context)
    {
        Context->Release();
    }

    if (Device)
    {
        Device->Release();
    }
    if (renderTargetTexture) renderTargetTexture->Release();
    if (viewportRenderTargetView) viewportRenderTargetView->Release();
    if (shaderResourceView) shaderResourceView->Release();
    if (depthState) depthState->Release();
    if (skyDepthState) skyDepthState->Release();
}


void DirectX11Graphics::RenderBucket(RenderingStats& stats, IShader* previousShader,
                                     std::map<IMaterial*, std::list<std::shared_ptr<IRenderable>>>::iterator bucket)
{
    if (bucket->first == nullptr)
    {
        return;
    }
    Context->OMSetDepthStencilState(depthState, 1);

    if (!bucket->first->Update())
    {
        return;
    }
    stats.materials++;

    MaterialProperties materialProperties;
    materialProperties = bucket->first->GetMaterialProperties();

    Context->UpdateSubresource(materialPropertiesConstantBuffer, 0, nullptr, &materialProperties, 0, 0);
    Context->PSSetConstantBuffers(0, 1, &materialPropertiesConstantBuffer);


    for (auto renderable = bucket->second.begin(); renderable != bucket->second.end(); ++renderable)
    {
        auto currentShader = bucket->first->GetShader();
        if (currentShader != previousShader)
        {
            previousShader = currentShader;
            stats.shaders++;
        }
        const auto renderObject = renderable->get();
        if (renderObject == nullptr || !renderObject->IsEnabled()) continue;

        if (renderObject == nullptr)
        {
            continue;
        }
        stats.tris += renderObject->GetTriangleCount();
        stats.verts += renderObject->GetVertsCount();
        stats.drawCalls++;

        const std::weak_ptr<Transform3D> transform = (*renderable)->GetTransform();
        SetMatrixBuffers(transform);
        Context->OMSetBlendState(BlendState, nullptr, ~0U);
        (*renderable)->Update();
    }
}

void DirectX11Graphics::RenderScene(std::shared_ptr<IRenderTarget> target)
{
    Context->OMSetRenderTargets(1, &target->RenderTargetView, target->DepthStencilView);
    //clear depth stencil for main frame regardless of target
    // Context->ClearDepthStencilView(windowRenderTarget->DepthStencilView, D3D11_CLEAR_DEPTH, 1, 0);
    Context->ClearDepthStencilView(target->DepthStencilView, D3D11_CLEAR_DEPTH, 1, 0);

    // if (renderToTexture)
    if (false)
    {
        float color = 32.0f / 255;
        float clearColour[4] = {color, color, color, 1.0f};
        float textureClearColour[4] = {color, color, color, 1.0f}; //{1.0f, 1.0f, 1.0f, 1.0f};
        Context->ClearRenderTargetView(viewportRenderTargetView, clearColour);
        //Context->ClearRenderTargetView(RenderTargetView, textureClearColour);
    }
    else
    {
        float clearColour[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        Context->ClearRenderTargetView(windowRenderTarget->RenderTargetView, clearColour);
        D3D11_VIEWPORT viewport;
        viewport.Width = static_cast<float>(target->Width);
        viewport.Height = static_cast<float>(target->Height);
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;
        viewport.TopLeftX = 0.0f;
        viewport.TopLeftY = 0.0f;
        Context->RSSetViewports(1, &viewport);
    }
    RenderingStats stats;
    stats.width = target->Width;
    stats.height = target->Height;
    // stats.viewportWidth = texWidth;
    // stats.viewportHeight = texHeight;
    stats.viewportWidth = target->Width;
    stats.viewportHeight = target->Height;
    IShader* previousShader = nullptr;
    PerFrameConstantBufferData constantBufferData;

    if (camera)
    {
        constantBufferData.ViewProjectionMatrix = camera->GetViewProjectionMatrix();
    }
    Context->UpdateSubresource(perFrameConstantBuffer, 0, nullptr, &constantBufferData, 0, 0);

    if (camera != nullptr)
    {
        auto pos = camera->GetTransform()->Position;
        lightProperties.EyePosition = XMFLOAT4(pos.X(), pos.Y(), pos.Z(), 1);
    }

    if (const auto& scene = SceneManager::GetScene().lock())
    {
        lightProperties.GlobalAmbient = XMFLOAT4(scene->GetAmbientLightColor().vec.x,
                                                 scene->GetAmbientLightColor().vec.y,
                                                 scene->GetAmbientLightColor().vec.z, 1.0f);
        int count = 0;
        const auto dirLight = scene->GetDirectionalLight();
        if (const auto dirLightShared = dirLight.lock())
        {
            lightProperties.Lights[count] = dirLightShared->GetLight();
            count++;
        }
        for (auto light : scene->GetLights())
        {
            lightProperties.Lights[count] = light->GetLight();
            count++;
            if (count == MAX_LIGHTS)
            {
                break;
            }
        }
    }
    Context->UpdateSubresource(lightPropertiesConstantBuffer, 0, nullptr, &lightProperties, 0, 0);
    Context->PSSetConstantBuffers(1, 1, &lightPropertiesConstantBuffer);

    for (auto bucket = Renderables.begin(); bucket != Renderables.end(); ++bucket)
    {
        if (bucket->first == nullptr)
        {
            continue;
        }

        RenderBucket(stats, previousShader, bucket);
    }
    currentStats = stats;
    for (auto strategy : postRenderStrategies)
    {
        if (strategy != nullptr)
        {
            strategy->Update();
        }
    }
    Context->OMSetRenderTargets(1, &windowRenderTarget->RenderTargetView, windowRenderTarget->DepthStencilView);
}

void DirectX11Graphics::Update()
{
    for (auto it = Renderables.begin(); it != Renderables.end();)
    {
        if (it->first == nullptr)
        {
            it = Renderables.erase(it);
        }
        else
        {
            ++it;
        }
    }

    if (Context && SwapChain)
    {
        for (auto target : RenderTargets)
        {
            auto targetLock = target.lock();
            RenderScene(targetLock);
        }
    }
}

void DirectX11Graphics::UpdateRenderable(IMaterial* mat, const std::shared_ptr<IRenderable>& renderable)
{
    RemoveRenderable(renderable);
    AddRenderable(mat, renderable);
}

void DirectX11Graphics::RemoveRenderable(const std::shared_ptr<IRenderable>& renderable)
{
    for (auto& kvp : Renderables)
    {
        std::list<std::shared_ptr<IRenderable>>& renderablesList = kvp.second;

        if (auto it = std::find(renderablesList.begin(), renderablesList.end(), renderable); it != renderablesList.
            end())
        {
            renderablesList.erase(it);

            if (renderablesList.empty())
            {
                Renderables.erase(kvp.first);
            }
            break;
        }
    }
}

void DirectX11Graphics::PostUpdate()
{
    SwapChain->Present(0, 0);
}

bool DirectX11Graphics::IsValid()
{
    return Device != nullptr;
}

ITexture* DirectX11Graphics::CreateTexture(const wchar_t* filepath)
{
    ITexture* Result = nullptr;
    ID3D11ShaderResourceView* Texture = nullptr;
    ID3D11SamplerState* Sampler = nullptr;
    D3D11_TEXTURE2D_DESC Description;

    if (IsValid())
    {
        HRESULT hr = CreateDDSTextureFromFile(Device, filepath, nullptr, &Texture);

        if (SUCCEEDED(hr))
        {
            D3D11_SAMPLER_DESC colorMapDesc;
            ZeroMemory(&colorMapDesc, sizeof(colorMapDesc));
            colorMapDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
            colorMapDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
            colorMapDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
            colorMapDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
            colorMapDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
            colorMapDesc.MaxLOD = D3D11_FLOAT32_MAX;

            hr = Device->CreateSamplerState(&colorMapDesc, &Sampler);
        }

        if (SUCCEEDED(hr))
        {
            ID3D11Resource* textureResource;
            Texture->GetResource(&textureResource);

            static_cast<ID3D11Texture2D*>(textureResource)->GetDesc(&Description);
            textureResource->Release();
        }

        if (SUCCEEDED(hr))
        {
            Result = new DirectX11Texture(filepath, Context, Texture, Sampler, Description);
            Textures.push_back(Result);
        }
    }

    return Result;
}


IShader* DirectX11Graphics::CreateShader(const wchar_t* filepath, const char* vsentry, const char* vsshader,
                                         const char* psentry, const char* psshader)
{
    IShader* Result = nullptr;
    ID3D11VertexShader* VertexShader = nullptr;
    ID3D11PixelShader* PixelShader = nullptr;
    ID3D11InputLayout* InputLayout = nullptr;


    if (IsValid())
    {
        if (TryCreateShaderData(filepath, vsentry, vsshader, psentry, psshader,
                                &VertexShader, &PixelShader, &InputLayout))
        {
            Result = new DirectX11Shader(filepath, Context, VertexShader, PixelShader, InputLayout);
        }
        else
        {
            Error("Unable to create shader")
        }
    }
    else
    {
        MessageBox(nullptr, "Invalid DirectX11Graphics", "Error!", MB_ICONEXCLAMATION | MB_OK);
    }

    return Result;
}

bool DirectX11Graphics::TryCreateShaderData(const wchar_t* filepath, const char* vsentry, const char* vsshader,
                                            const char* psentry, const char* psshader,
                                            ID3D11VertexShader** VertexShader,
                                            ID3D11PixelShader** PixelShader, ID3D11InputLayout** InputLayout)
{
    HRESULT hr = S_FALSE;
    ID3DBlob* vsBuffer = nullptr;
    ID3DBlob* psBuffer = nullptr;
    // Compile the vertex shader
    if (CompileShader(filepath, vsentry, vsshader, &vsBuffer))
    {
        hr = Device->CreateVertexShader(vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), nullptr,
                                        VertexShader);
        if (FAILED(hr))
        {
            MessageBox(nullptr, "Failed to create vertex shader", "Error!", MB_ICONEXCLAMATION | MB_OK);
            vsBuffer->Release();
            return false;
        }

        D3D11_INPUT_ELEMENT_DESC layout[] =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0},
        };
        unsigned int totalLayoutElements = ARRAYSIZE(layout);

        hr = Device->CreateInputLayout(layout, totalLayoutElements, vsBuffer->GetBufferPointer(),
                                       vsBuffer->GetBufferSize(), InputLayout);
        vsBuffer->Release();
        if (FAILED(hr))
        {
            MessageBox(nullptr, "Failed to create input layout", "Error!", MB_ICONEXCLAMATION | MB_OK);
            if (*VertexShader) (*VertexShader)->Release(); // Release VertexShader if it was created
            return false;
        }
    }
    else
    {
        MessageBox(nullptr, "Failed to compile vertex shader", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return false;
    }

    if (CompileShader(filepath, psentry, psshader, &psBuffer))
    {
        hr = Device->CreatePixelShader(psBuffer->GetBufferPointer(), psBuffer->GetBufferSize(), nullptr,
                                       PixelShader);
        psBuffer->Release();
        if (FAILED(hr))
        {
            MessageBox(nullptr, "Failed to create pixel shader", "Error!", MB_ICONEXCLAMATION | MB_OK);
            if (*VertexShader) (*VertexShader)->Release();
            if (*InputLayout) (*InputLayout)->Release();
            return false;
        }
    }
    else
    {
        MessageBox(nullptr, "Failed to compile pixel shader", "Error!", MB_ICONEXCLAMATION | MB_OK);
        if (*VertexShader) (*VertexShader)->Release();
        if (*InputLayout) (*InputLayout)->Release();
        return false;
    }
    return true;
}

std::shared_ptr<IRenderable> DirectX11Graphics::CreateBillboard(IMaterial* material)
{
    std::shared_ptr<IRenderable> Result = nullptr;

    if (IsValid())
    {
        float halfWidth = 1.0f;
        float halfHeight = 1.0f;

        if (material != nullptr)
        {
            ITexture* texture = material->GetTexture();
            if (texture != nullptr)
            {
                halfWidth = texture ? texture->GetWidth() / 2.0f : 0.5f;
                halfHeight = texture ? texture->GetHeight() / 2.0f : 0.5f;
            }
        }

        float vertex_data_array[] =
        {
            // Positions                    // Colors                // Normals           // UVs
            halfWidth, halfHeight, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            halfWidth, -halfHeight, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            -halfWidth, -halfHeight, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            -halfWidth, halfHeight, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f
        };

        ID3D11Buffer* VertexBuffer;
        unsigned int vertexStride = 12 * sizeof(float);
        unsigned int vertexOffset = 0;
        unsigned int vertexCount = 4;

        D3D11_BUFFER_DESC vertexDescription;
        ZeroMemory(&vertexDescription, sizeof(vertexDescription));
        vertexDescription.Usage = D3D11_USAGE_DEFAULT;
        vertexDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertexDescription.ByteWidth = sizeof(vertex_data_array);

        D3D11_SUBRESOURCE_DATA resourceData;
        ZeroMemory(&resourceData, sizeof(resourceData));
        resourceData.pSysMem = vertex_data_array;

        unsigned int indexBufferData[] =
        {
            0, 1, 2, 2, 3, 0
        };
        unsigned int indexCount = 6;

        ID3D11Buffer* IndexBuffer;
        D3D11_BUFFER_DESC indexDescription;
        ZeroMemory(&indexDescription, sizeof(indexDescription));
        indexDescription.Usage = D3D11_USAGE_DEFAULT;
        indexDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
        indexDescription.ByteWidth = sizeof(indexBufferData);

        D3D11_SUBRESOURCE_DATA indexResourceData;
        ZeroMemory(&indexResourceData, sizeof(indexResourceData));
        indexResourceData.pSysMem = indexBufferData;

        if (SUCCEEDED(Device->CreateBuffer(&vertexDescription, &resourceData, &VertexBuffer)) &&
            SUCCEEDED(Device->CreateBuffer(&indexDescription, &indexResourceData, &IndexBuffer)))
        {
            Result = std::make_shared<DirectX11Billboard>(Context, VertexBuffer, IndexBuffer, vertexStride,
                                                          vertexOffset, vertexCount, indexCount);
            AddRenderable(material, Result);
        }
    }

    return Result;
}

void DirectX11Graphics::AddRenderable(IMaterial* material, std::shared_ptr<IRenderable> Result)
{
    if (!Renderables.contains(material))
    {
        Renderables.insert(std::pair(material, std::list<std::shared_ptr<IRenderable>>()));
    }
    Renderables[material].push_back(Result);
}

std::shared_ptr<IMeshRenderable> DirectX11Graphics::CreateMeshRenderable(IMaterial* material, Mesh* mesh)
{
    std::shared_ptr<IMeshRenderable> Result = nullptr;

    if (IsValid() && mesh)
    {
        ID3D11Buffer* VertexBuffer;
        unsigned int vertexStride = sizeof(Vertex);
        unsigned int vertexOffset = 0;
        unsigned int vertexCount = static_cast<unsigned int>(mesh->GetVerts().size());

        D3D11_BUFFER_DESC vertexDescription;
        ZeroMemory(&vertexDescription, sizeof(vertexDescription));
        vertexDescription.Usage = D3D11_USAGE_DEFAULT;
        vertexDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertexDescription.ByteWidth = static_cast<UINT>(vertexCount * sizeof(Vertex));

        D3D11_SUBRESOURCE_DATA resourceData;
        ZeroMemory(&resourceData, sizeof(resourceData));
        resourceData.pSysMem = mesh->GetVerts().data();

        unsigned int indexCount = static_cast<unsigned int>(mesh->GetIndices().size());

        ID3D11Buffer* IndexBuffer;
        D3D11_BUFFER_DESC indexDescription;
        ZeroMemory(&indexDescription, sizeof(indexDescription));
        indexDescription.Usage = D3D11_USAGE_DEFAULT;
        indexDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
        indexDescription.ByteWidth = static_cast<UINT>(indexCount * sizeof(unsigned int));

        D3D11_SUBRESOURCE_DATA indexResourceData;
        ZeroMemory(&indexResourceData, sizeof(indexResourceData));
        indexResourceData.pSysMem = mesh->GetIndices().data();

        if (SUCCEEDED(Device->CreateBuffer(&vertexDescription, &resourceData, &VertexBuffer)) &&
            SUCCEEDED(Device->CreateBuffer(&indexDescription, &indexResourceData, &IndexBuffer)))
        {
            Result = std::make_shared<DirectX11Mesh>(mesh->GetPath(), mesh->GetAABB(), Context, VertexBuffer,
                                                     IndexBuffer, vertexStride,
                                                     vertexOffset, vertexCount, indexCount);
            AddRenderable(material, Result);
        }
    }

    return Result;
}

std::shared_ptr<IMeshRenderable> DirectX11Graphics::CreateMeshRenderable(Mesh* mesh)
{
    std::shared_ptr<IMeshRenderable> Result = nullptr;

    if (IsValid() && mesh)
    {
        ID3D11Buffer* VertexBuffer;
        unsigned int vertexStride = sizeof(Vertex);
        unsigned int vertexOffset = 0;
        unsigned int vertexCount = static_cast<unsigned int>(mesh->GetVerts().size());

        D3D11_BUFFER_DESC vertexDescription;
        ZeroMemory(&vertexDescription, sizeof(vertexDescription));
        vertexDescription.Usage = D3D11_USAGE_DEFAULT;
        vertexDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertexDescription.ByteWidth = static_cast<UINT>(vertexCount * sizeof(Vertex));

        D3D11_SUBRESOURCE_DATA resourceData;
        ZeroMemory(&resourceData, sizeof(resourceData));
        resourceData.pSysMem = mesh->GetVerts().data();

        unsigned int indexCount = static_cast<unsigned int>(mesh->GetIndices().size());

        ID3D11Buffer* IndexBuffer;
        D3D11_BUFFER_DESC indexDescription;
        ZeroMemory(&indexDescription, sizeof(indexDescription));
        indexDescription.Usage = D3D11_USAGE_DEFAULT;
        indexDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
        indexDescription.ByteWidth = static_cast<UINT>(indexCount * sizeof(unsigned int));

        D3D11_SUBRESOURCE_DATA indexResourceData;
        ZeroMemory(&indexResourceData, sizeof(indexResourceData));
        indexResourceData.pSysMem = mesh->GetIndices().data();

        if (SUCCEEDED(Device->CreateBuffer(&vertexDescription, &resourceData, &VertexBuffer)) &&
            SUCCEEDED(Device->CreateBuffer(&indexDescription, &indexResourceData, &IndexBuffer)))
        {
            Result = std::make_shared<DirectX11Mesh>(mesh->GetPath(), mesh->GetAABB(), Context, VertexBuffer,
                                                     IndexBuffer, vertexStride,
                                                     vertexOffset, vertexCount, indexCount);
            AddRenderable(ResourceManager::GetMaterial(), Result);
        }
    }

    return Result;
}

void DirectX11Graphics::SetActiveCamera(std::shared_ptr<ICamera> camera)
{
    this->camera = camera;
}

void DirectX11Graphics::SetRenderToTexture(int width, int height)
{
    auto texWidth = width;
    auto texHeight = height;

    if (renderTargetTexture) renderTargetTexture->Release();
    if (viewportRenderTargetView) viewportRenderTargetView->Release();
    if (shaderResourceView) shaderResourceView->Release();

    if (textureTargetDepthStencilBuffer) textureTargetDepthStencilBuffer->Release();
    if (textureTargetDepthStencilView) textureTargetDepthStencilView->Release();

    D3D11_VIEWPORT textureViewport;
    textureViewport.TopLeftX = 0.0f;
    textureViewport.TopLeftY = 0.0f;
    textureViewport.Width = static_cast<float>(texWidth);
    textureViewport.Height = static_cast<float>(texHeight);
    textureViewport.MinDepth = 0.0f;
    textureViewport.MaxDepth = 1.0f;
    Context->RSSetViewports(1, &textureViewport);

    D3D11_TEXTURE2D_DESC textureDesc;
    ZeroMemory(&textureDesc, sizeof(textureDesc));
    textureDesc.Width = texWidth;
    textureDesc.Height = texHeight;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    Device->CreateTexture2D(&textureDesc, nullptr, &renderTargetTexture);
    Device->CreateRenderTargetView(renderTargetTexture, nullptr, &viewportRenderTargetView);
    Device->CreateShaderResourceView(renderTargetTexture, nullptr, &shaderResourceView);

    D3D11_TEXTURE2D_DESC depthDesc;
    ZeroMemory(&depthDesc, sizeof(depthDesc));
    depthDesc.Width = width;
    depthDesc.Height = height;
    depthDesc.MipLevels = 1;
    depthDesc.ArraySize = 1;
    depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthDesc.SampleDesc.Count = 1;
    depthDesc.Usage = D3D11_USAGE_DEFAULT;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    Device->CreateTexture2D(&depthDesc, nullptr, &textureTargetDepthStencilBuffer);

    Device->CreateDepthStencilView(textureTargetDepthStencilBuffer, nullptr, &textureTargetDepthStencilView);

    Context->OMSetRenderTargets(1, &viewportRenderTargetView, textureTargetDepthStencilView);
    if (camera != nullptr)
    {
        camera->SetWidth(static_cast<float>(texWidth));
        camera->SetHeight(static_cast<float>(texHeight));
    }
}

ID3D11ShaderResourceView* DirectX11Graphics::GetTextureView() const
{
    return shaderResourceView;
}

ID3D11Texture2D* DirectX11Graphics::GetTexture() const
{
    return renderTargetTexture;
}

IMaterial* DirectX11Graphics::CreateMaterial(IShader* shader, ITexture* texture)
{
    return new DirectX11Material(shader, texture);
}

void DirectX11Graphics::WindowResize(int newWidth, int newHeight)
{
    if (Device == nullptr || newWidth == 0 || newHeight == 0) return;
    windowRenderTarget->Resize(newWidth, newHeight);
    
    
    UpdateRenderToTextureResources(newWidth, newHeight);
}

void DirectX11Graphics::UpdateRenderToTextureResources(int newWidth, int newHeight)
{
    // if (renderToTexture)
    if(false)
    {
        if (renderTargetTexture) renderTargetTexture->Release();
        if (viewportRenderTargetView) viewportRenderTargetView->Release();
        if (shaderResourceView) shaderResourceView->Release();

        D3D11_TEXTURE2D_DESC textureDesc;
        ZeroMemory(&textureDesc, sizeof(textureDesc));
        textureDesc.Width = newWidth;
        textureDesc.Height = newHeight;
        textureDesc.MipLevels = 1;
        textureDesc.ArraySize = 1;
        textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        Device->CreateTexture2D(&textureDesc, nullptr, &renderTargetTexture);
        Device->CreateRenderTargetView(renderTargetTexture, nullptr, &viewportRenderTargetView);
        Device->CreateShaderResourceView(renderTargetTexture, nullptr, &shaderResourceView);
        
        if (camera)
        {
            camera->SetWidth(static_cast<float>(newWidth));
            camera->SetHeight(static_cast<float>(newHeight));
        }
    }
}

bool DirectX11Graphics::TryUpdateShader(IShader* iShader, const char* vsentry, const char* vsshader,
                                        const char* psentry,
                                        const char* psshader)
{
    ID3D11VertexShader* VertexShader = nullptr;
    ID3D11PixelShader* PixelShader = nullptr;
    ID3D11InputLayout* InputLayout = nullptr;

    const auto shader = dynamic_cast<DirectX11Shader*>(iShader);
    if (IsValid() && shader)
    {
        if (TryCreateShaderData(shader->GetPath().c_str(), vsentry, vsshader, psentry, psshader,
                                &VertexShader, &PixelShader, &InputLayout))
        {
            shader->SetVertexShader(VertexShader);
            shader->SetPixelShader(PixelShader);
            shader->SetInputLayout(InputLayout);
        }
        else
        {
            Error("Unable to update shader")
            return false;
        }
    }
    else
    {
        MessageBox(nullptr, "Invalid DirectX11Graphics", "Error!", MB_ICONEXCLAMATION | MB_OK);
    }
    return true;
}

IDXGISwapChain* DirectX11Graphics::GetSwapChain() const
{
    return SwapChain;
}


void DirectX11Graphics::SetMatrixBuffers(const std::weak_ptr<Transform3D> transform)
{
    if (std::shared_ptr<Transform3D> trans = transform.lock())
    {
        XMMATRIX translation = XMMatrixTranslation(trans->Position.X(), trans->Position.Y(),
                                                   trans->Position.Z());
        XMMATRIX rotation = XMMatrixRotationRollPitchYaw(
            XMConvertToRadians(trans->Rotation.X())
            , XMConvertToRadians(trans->Rotation.Y()), XMConvertToRadians(trans->Rotation.Z()));
        XMMATRIX scale = XMMatrixScaling(trans->Scale.X(), trans->Scale.Y(), trans->Scale.Z());

        XMMATRIX model = scale * rotation * translation;


        PerObjectConstantBufferData perObjectConstantBufferData;
        perObjectConstantBufferData.WorldMatrix = model;
        perObjectConstantBufferData.InverseTransposeWorldMatrix = XMMatrixTranspose(XMMatrixInverse(nullptr, model));
        perObjectConstantBufferData.WorldViewProjectionMatrix = model * camera->GetViewProjectionMatrix();
        Context->UpdateSubresource(perObjectConstantBuffer, 0, nullptr, &perObjectConstantBufferData, 0, 0);
        Context->VSSetConstantBuffers(0, 1, &perObjectConstantBuffer);
    }
}

bool DirectX11Graphics::CompileShader(LPCWSTR filepath, LPCSTR entry, LPCSTR shader, ID3DBlob** buffer)
{
    DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#ifdef _DEBUG
    shaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* errorBuffer = 0;
    HRESULT hr = D3DCompileFromFile(filepath, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, entry, shader, shaderFlags, 0,
                                    buffer, &errorBuffer);

    if (FAILED(hr))
    {
        if (errorBuffer)
        {
            MessageBox(nullptr, static_cast<char*>(errorBuffer->GetBufferPointer()), "Error!",
                       MB_ICONEXCLAMATION | MB_OK);
        }
    }

    if (errorBuffer)
    {
        errorBuffer->Release();
    }

    return hr == S_OK;
}
