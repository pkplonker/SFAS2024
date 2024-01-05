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
#include "IApplication.h"
#include "RenderingStats.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Engine/Implementation/Logging/Debug.h"
#include "Implementation/Mesh.h"
#include "Implementation/Scene.h"
#include "Implementation/Vertex.h"

struct
{
    DirectX::XMMATRIX mvp;
    DirectX::XMMATRIX worldMatrix;
    DirectX::XMFLOAT3 ambientLightColor;
    float ambientLightIntensity;
} matrices;


struct DirectionalLightBufferObject
{
    Vec4 direction = Vec4(0, -1, 0, 1);
    Vec4 color = Vec4(1, 1, 1, 1);
    float intensity = 1;
    float padding[3] = {0, 0, 0};
};

DirectX11Graphics::DirectX11Graphics(HWND hwndIn) : Device(nullptr), Context(nullptr), SwapChain(nullptr),
                                                    BackbufferView(nullptr), BackbufferTexture(nullptr), Mvp(nullptr),
                                                    vpMatrix(), FeatureLevel(D3D_FEATURE_LEVEL_11_0), hwnd(hwndIn),
                                                    width(0), height(0), texWidth(0), texHeight(0),
                                                    renderToTexture(false)
{
    RECT dimensions;
    GetClientRect(hwnd, &dimensions);
    width = dimensions.right - dimensions.left;
    height = dimensions.bottom - dimensions.top;

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
        hr = D3D11CreateDeviceAndSwapChain(nullptr, driverTypes[count], NULL, creationFlags, NULL, 0, D3D11_SDK_VERSION,
                                           &sd, &SwapChain, &Device, &FeatureLevel, &Context);

        if (SUCCEEDED(hr))
        {
            break;
        }
    }

    if (SUCCEEDED(hr))
    {
        hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&BackbufferTexture);
    }

    if (SUCCEEDED(hr))
    {
        hr = Device->CreateRenderTargetView(BackbufferTexture, 0, &BackbufferView);
    }

    if (BackbufferTexture)
    {
        BackbufferTexture->Release();
    }

    if (FAILED(hr))
    {
        MessageBox(nullptr, "Graphics Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
    }
    else
    {
        D3D11_BUFFER_DESC constDesc;
        ZeroMemory(&constDesc, sizeof(constDesc));
        constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        constDesc.ByteWidth = sizeof(matrices);
        constDesc.Usage = D3D11_USAGE_DEFAULT;
        hr = Device->CreateBuffer(&constDesc, 0, &Mvp);

        if (FAILED(hr))
        {
            MessageBox(nullptr, "Graphics Failed to create MVP Buffer", "Error!", MB_ICONEXCLAMATION | MB_OK);
        }

        if (camera == nullptr)
        {
            DirectX::XMVECTOR EyePosition = DirectX::XMVectorSet(0.0f, 0.0f, -10.0f, 1.0f);
            DirectX::XMVECTOR FocusPoint = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
            DirectX::XMVECTOR UpDirection = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
            DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(EyePosition, FocusPoint, UpDirection);
            float fovAngleY = DirectX::XM_PIDIV4;
            float aspectRatio = static_cast<float>(width) / height;
            float nearZ = 0.1f;
            float farZ = 2000.0f;
            DirectX::XMMATRIX projection = DirectX::XMMatrixPerspectiveFovLH(
                fovAngleY,
                aspectRatio,
                nearZ,
                farZ
            );
            vpMatrix = XMMatrixMultiply(view, projection);
        }
        else
        {
            vpMatrix = camera->GetViewProjectionMatrix();
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
        Device->CreateDepthStencilView(depthStencil, &depthViewDesc, &DepthStencilView);

        Context->OMSetRenderTargets(1, &BackbufferView, DepthStencilView);

        ZeroMemory(&materialBufferDesc, sizeof(D3D11_BUFFER_DESC));
        materialBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        materialBufferDesc.ByteWidth = sizeof(MaterialBufferObject);
        materialBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        materialBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        Device->CreateBuffer(&materialBufferDesc, nullptr, &materialBuffer);

        ZeroMemory(&directionalLightBufferDesc, sizeof(D3D11_BUFFER_DESC));
        directionalLightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        directionalLightBufferDesc.ByteWidth = sizeof(DirectionalLightBufferObject);
        directionalLightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        directionalLightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        Device->CreateBuffer(&directionalLightBufferDesc, nullptr, &directionalLightBuffer);
    }
}

DirectX11Graphics::~DirectX11Graphics()
{
    if (BlendState)
    {
        BlendState->Release();
    }

    if (BackbufferView)
    {
        BackbufferView->Release();
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
    if (renderTargetView) renderTargetView->Release();
    if (shaderResourceView) shaderResourceView->Release();
    if (depthState) depthState->Release();
    if (skyDepthState) skyDepthState->Release();
    if (directionalLightBuffer) directionalLightBuffer->Release();
    if (materialBuffer) materialBuffer->Release();
}


void DirectX11Graphics::RenderBucket(RenderingStats& stats, IShader* previousShader,
                                     std::map<IMaterial*, std::list<std::shared_ptr<IRenderable>>>::iterator bucket)
{
    if (bucket->first == nullptr)
    {
        return;
    }
    bucket->first->GetIsSkybox()
        ? Context->OMSetDepthStencilState(skyDepthState, 1)
        : Context->OMSetDepthStencilState(depthState, 1);

    if (!bucket->first->Update())
    {
        return;
    }
    stats.materials++;

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
        {
            // Currently setting the material buffer per object, which is no doubt inefficient. This likely needs splitting to be grouped, shader->texture->material values?
            D3D11_MAPPED_SUBRESOURCE mappedResource;
            Context->Map(materialBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
            auto* data = static_cast<MaterialBufferObject*>(mappedResource.pData);
            bucket->first->UpdateMaterialBuffer(data);
            Context->Unmap(materialBuffer, 0);
            Context->PSSetConstantBuffers(1, 1, &materialBuffer);
            Context->VSSetConstantBuffers(1, 1, &materialBuffer);
        }
        const std::weak_ptr<Transform3D> transform = (*renderable)->GetTransform();
        SetMatrixBuffers(transform);
        Context->OMSetBlendState(BlendState, nullptr, ~0U);
        (*renderable)->Update();
    }
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
        ID3D11RenderTargetView* activeRenderTarget = renderToTexture ? renderTargetView : BackbufferView;
        ID3D11DepthStencilView* activedepthView = renderToTexture ? textureTargetDepthStencilView : DepthStencilView;

        Context->OMSetRenderTargets(1, &activeRenderTarget, activedepthView);
        //clear depth stencil for main frame regardless of target
        Context->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH, 1, 0);
        Context->ClearDepthStencilView(activedepthView, D3D11_CLEAR_DEPTH, 1, 0);
        if (camera != nullptr)
        {
            vpMatrix = camera->GetViewProjectionMatrix();
        }

        if (renderToTexture)
        {
            float color = 32.0f / 255;
            float clearColour[4] = {color, color, color, 1.0f};
            float textureClearColour[4] = {color, color, color, 1.0f}; //{1.0f, 1.0f, 1.0f, 1.0f};
            Context->ClearRenderTargetView(renderTargetView, clearColour);
            Context->ClearRenderTargetView(BackbufferView, textureClearColour);
        }
        else
        {
            float clearColour[4] = {1.0f, 1.0f, 1.0f, 1.0f};
            Context->ClearRenderTargetView(BackbufferView, clearColour);
            D3D11_VIEWPORT viewport;
            viewport.Width = static_cast<float>(width);
            viewport.Height = static_cast<float>(height);
            viewport.MinDepth = 0.0f;
            viewport.MaxDepth = 1.0f;
            viewport.TopLeftX = 0.0f;
            viewport.TopLeftY = 0.0f;
            Context->RSSetViewports(1, &viewport);
        }
        RenderingStats stats;
        stats.width = width;
        stats.height = height;
        stats.viewportWidth = texWidth;
        stats.viewportHeight = texHeight;
        IShader* previousShader = nullptr;
        SetDirectionalLightBuffers();
        for (auto bucket = Renderables.begin(); bucket != Renderables.end(); ++bucket)
        {
            if (bucket->first == nullptr || !bucket->first->GetIsSkybox())
            {
                continue;
            }

            Context->OMSetDepthStencilState(skyDepthState, 1);

            RenderBucket(stats, previousShader, bucket);
        }
        for (auto bucket = Renderables.begin(); bucket != Renderables.end(); ++bucket)
        {
            if (bucket->first == nullptr || bucket->first->GetIsSkybox())
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
        Context->OMSetRenderTargets(1, &BackbufferView, DepthStencilView);
    }
}

void DirectX11Graphics::SetDirectionalLightBuffers()
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    Context->Map(directionalLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    auto* data = static_cast<DirectionalLightBufferObject*>(mappedResource.pData);
    data->color = Vec4(1, 0, 0, 1);
    data->direction = Vec4(0, 1, 0, 1);
    if (const auto scene = SceneManager::GetScene().lock())
    {
        auto dirLight = scene->GetDirectionalLight();
        if (const auto& light = dirLight.lock())
        {
            data->color = light->GetColor();
            data->direction = light->GetDirection();
            data->intensity = light->intensity;
        }
    }
    Context->Unmap(directionalLightBuffer, 0);
    Context->PSSetConstantBuffers(2, 1, &directionalLightBuffer);
    Context->VSSetConstantBuffers(2, 1, &directionalLightBuffer);
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
        HRESULT hr = DirectX::CreateDDSTextureFromFile(Device, filepath, nullptr, &Texture);

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

void DirectX11Graphics::SetRenderToTexture(bool state, int width, int height)
{
    if (state)
    {
        texWidth = width;
        texHeight = height;

        if (renderTargetTexture) renderTargetTexture->Release();
        if (renderTargetView) renderTargetView->Release();
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
        Device->CreateRenderTargetView(renderTargetTexture, nullptr, &renderTargetView);
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

        Context->OMSetRenderTargets(1, &renderTargetView, textureTargetDepthStencilView);
        if (camera != nullptr)
        {
            camera->SetWidth(static_cast<float>(texWidth));
            camera->SetHeight(static_cast<float>(texHeight));
        }
    }
    renderToTexture = state;
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

void DirectX11Graphics::Resize(int newWidth, int newHeight)
{
    if (Device == nullptr || newWidth == 0 || newHeight == 0) return;

    Context->OMSetRenderTargets(0, nullptr, nullptr);
    if (BackbufferView)
    {
        BackbufferView->Release();
        BackbufferView = nullptr;
    }
    if (DepthStencilView)
    {
        DepthStencilView->Release();
        DepthStencilView = nullptr;
    }

    HRESULT hr = SwapChain->ResizeBuffers(0, newWidth, newHeight, DXGI_FORMAT_UNKNOWN, 0);
    if (FAILED(hr))
    {
        Error("Failed resizing swapchain buffersr")
    }

    ID3D11Texture2D* backBuffer = nullptr;
    hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
    if (SUCCEEDED(hr))
    {
        hr = Device->CreateRenderTargetView(backBuffer, nullptr, &BackbufferView);
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
    hr = Device->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencilBuffer);
    if (SUCCEEDED(hr))
    {
        hr = Device->CreateDepthStencilView(depthStencilBuffer, nullptr, &DepthStencilView);
        depthStencilBuffer->Release();
    }
    if (FAILED(hr))
    {
        Error("Failed modifying depth buffer")
    }

    Context->OMSetRenderTargets(1, &BackbufferView, DepthStencilView);

    D3D11_VIEWPORT viewport = {};
    viewport.Width = static_cast<float>(newWidth);
    viewport.Height = static_cast<float>(newHeight);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    Context->RSSetViewports(1, &viewport);

    this->width = newWidth;
    this->height = newHeight;
    UpdateRenderToTextureResources(newWidth, newHeight);
}

void DirectX11Graphics::UpdateRenderToTextureResources(int newWidth, int newHeight)
{
    if (renderToTexture)
    {
        if (renderTargetTexture) renderTargetTexture->Release();
        if (renderTargetView) renderTargetView->Release();
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
        Device->CreateRenderTargetView(renderTargetTexture, nullptr, &renderTargetView);
        Device->CreateShaderResourceView(renderTargetTexture, nullptr, &shaderResourceView);

        texWidth = newWidth;
        texHeight = newHeight;

        if (camera)
        {
            camera->SetWidth(static_cast<float>(texWidth));
            camera->SetHeight(static_cast<float>(texHeight));
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

void DirectX11Graphics::AmbientLightBufferUpdate()
{
    if (const auto& scene = SceneManager::GetScene().lock())
    {
        matrices.ambientLightColor = scene->GetAmbientLightColor();
        matrices.ambientLightIntensity = scene->GetAmbientLightIntensity();
    }else
    {
        matrices.ambientLightColor = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
        matrices.ambientLightIntensity = 1.0f;
    }
}

void DirectX11Graphics::SetMatrixBuffers(const std::weak_ptr<Transform3D> transform)
{
    if (std::shared_ptr<Transform3D> trans = transform.lock())
    {
        DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(trans->Position.X(), trans->Position.Y(),
                                                                     trans->Position.Z());
        DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYaw(
            DirectX::XMConvertToRadians(trans->Rotation.X())
            , DirectX::XMConvertToRadians(trans->Rotation.Y()), DirectX::XMConvertToRadians(trans->Rotation.Z()));
        DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(trans->Scale.X(), trans->Scale.Y(), trans->Scale.Z());

        DirectX::XMMATRIX world = scale * rotation * translation;
        DirectX::XMMATRIX mvp = DirectX::XMMatrixMultiply(world, vpMatrix);

        world = DirectX::XMMatrixTranspose(world);
        mvp = DirectX::XMMatrixTranspose(mvp);


        matrices.mvp = mvp;
        matrices.worldMatrix = world;
        AmbientLightBufferUpdate();


        Context->UpdateSubresource(Mvp, 0, 0, &matrices, 0, 0);
        Context->VSSetConstantBuffers(0, 1, &Mvp);
        Context->PSSetConstantBuffers(0, 1, &Mvp);
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
