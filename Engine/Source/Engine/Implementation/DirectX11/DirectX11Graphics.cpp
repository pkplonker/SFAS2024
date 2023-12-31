#include "DirectX11Graphics.h"

#include "DirectX11Billboard.h"
#include "DirectX11Shader.h"
#include "DirectX11Texture.h"
#include <d3dcommon.h>
#include <d3d11.h>
#include <D3DCompiler.h>
#include <DDSTextureLoader.h>
#include <memory>

#include "DirectX11Mesh.h"
#include "Engine/Implementation/Debug.h"
#include "Implementation/Mesh.h"
#include "Implementation/Vertex.h"

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
        constDesc.ByteWidth = sizeof(DirectX::XMMATRIX);
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
        ID3D11DepthStencilState* depthState;
        Device->CreateDepthStencilState(&depthStencildesc, &depthState);
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
}

void DirectX11Graphics::Update()
{
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
            float textureClearColour[4] = {1.0f, 1.0f, 1.0f, 1.0f};
            Context->ClearRenderTargetView(renderTargetView, textureClearColour);
            Context->ClearRenderTargetView(BackbufferView, clearColour);
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

        for (auto bucket = Renderables.begin(); bucket != Renderables.end(); ++bucket)
        {
            bucket->first->Update();

            for (auto renderable = bucket->second.begin(); renderable != bucket->second.end(); ++renderable)
            {
                std::weak_ptr<Transform3D> transform = (*renderable)->GetTransform();
                SetWorldMatrix(transform);
                Context->OMSetBlendState(BlendState, nullptr, ~0U);
                (*renderable)->Update();
            }
        }


        Context->OMSetRenderTargets(1, &BackbufferView, DepthStencilView);
    }
}

void DirectX11Graphics::RemoveRenderable(const std::shared_ptr<IRenderable>& shared)
{
    for (auto& kvp : Renderables)
    {
        std::list<std::shared_ptr<IRenderable>>& renderablesList = kvp.second;

        if (auto it = std::find(renderablesList.begin(), renderablesList.end(), shared); it != renderablesList.end())
        {
            renderablesList.erase(it);

            if (renderablesList.empty())
            {
                IShader* shader = kvp.first;
                Renderables.erase(shader);
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
            Result = new DirectX11Texture(Context, Texture, Sampler, Description);
            Textures.push_back(Result);
        }
    }

    return Result;
}

IShader* DirectX11Graphics::CreateShader(const wchar_t* filepath, const char* vsentry, const char* vsshader,
                                         const char* psentry, const char* psshader, ITexture* TextureIn)
{
    IShader* Result = nullptr;
    ID3D11VertexShader* VertexShader = nullptr;
    ID3D11PixelShader* PixelShader = nullptr;
    ID3D11InputLayout* InputLayout = nullptr;
    HRESULT hr = S_FALSE;
    ID3DBlob* vsBuffer = nullptr;
    ID3DBlob* psBuffer = nullptr;

    if (IsValid())
    {
        // Compile the vertex shader
        if (CompileShader(filepath, vsentry, vsshader, &vsBuffer))
        {
            hr = Device->CreateVertexShader(vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), nullptr,
                                            &VertexShader);
            if (FAILED(hr))
            {
                MessageBox(nullptr, "Failed to create vertex shader", "Error!", MB_ICONEXCLAMATION | MB_OK);
                vsBuffer->Release();
                return nullptr;
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
                                           vsBuffer->GetBufferSize(), &InputLayout);
            vsBuffer->Release();
            if (FAILED(hr))
            {
                MessageBox(nullptr, "Failed to create input layout", "Error!", MB_ICONEXCLAMATION | MB_OK);
                VertexShader->Release();
                return nullptr;
            }
        }
        else
        {
            MessageBox(nullptr, "Failed to compile vertex shader", "Error!", MB_ICONEXCLAMATION | MB_OK);
            return nullptr;
        }

        if (CompileShader(filepath, psentry, psshader, &psBuffer))
        {
            hr = Device->CreatePixelShader(psBuffer->GetBufferPointer(), psBuffer->GetBufferSize(), nullptr,
                                           &PixelShader);
            psBuffer->Release();
            if (FAILED(hr))
            {
                MessageBox(nullptr, "Failed to create pixel shader", "Error!", MB_ICONEXCLAMATION | MB_OK);
                VertexShader->Release();
                InputLayout->Release();
                return nullptr;
            }
        }
        else
        {
            MessageBox(nullptr, "Failed to compile pixel shader", "Error!", MB_ICONEXCLAMATION | MB_OK);
            VertexShader->Release();
            InputLayout->Release();
            return nullptr;
        }

        Result = new DirectX11Shader(Context, VertexShader, PixelShader, InputLayout, TextureIn);
        Renderables.insert(std::pair(Result, std::list<std::shared_ptr<IRenderable>>()));
    }
    else
    {
        MessageBox(nullptr, "Invalid DirectX11Graphics", "Error!", MB_ICONEXCLAMATION | MB_OK);
    }

    return Result;
}


std::shared_ptr<IRenderable> DirectX11Graphics::CreateBillboard(IShader* ShaderIn)
{
    std::shared_ptr<IRenderable> Result = nullptr;

    if (IsValid())
    {
        const ITexture* texture = ShaderIn->GetTexture();
        const float halfWidth = texture ? texture->GetWidth() / 2.0f : 0.5f;
        const float halfHeight = texture ? texture->GetHeight() / 2.0f : 0.5f;

        float vertex_data_array[] =
        {
            halfWidth, halfHeight, 0.0f, 1.0f, 1.0f,
            halfWidth, -halfHeight, 0.0f, 1.0f, 0.0f,
            -halfWidth, -halfHeight, 0.0f, 0.0f, 0.0f,

            -halfWidth, halfHeight, 0.0f, 0.0f, 1.0f,
        };
        ID3D11Buffer* VertexBuffer;
        unsigned int vertexStride = 5 * sizeof(float);
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

        if (SUCCEEDED(Device->CreateBuffer(&vertexDescription, &resourceData, &VertexBuffer)) && SUCCEEDED(
            Device->CreateBuffer(&indexDescription, &indexResourceData, &IndexBuffer)))
        {
            Result = std::make_shared<DirectX11Billboard>(Context, VertexBuffer, IndexBuffer, vertexStride,
                                                          vertexOffset,
                                                          vertexCount, indexCount);
            Renderables[ShaderIn].push_back(Result);
        }
    }

    return Result;
}

std::shared_ptr<IRenderable> DirectX11Graphics::CreateMeshRenderable(IShader* ShaderIn)
{
    std::shared_ptr<IRenderable> Result = nullptr;

    if (IsValid())
    {
        float size = .5f;


        float vertex_data_array[] = {
            -size, -size, -size, 0.0f, 0.0f,
            -size, size, -size, 0.0f, 0.0f,
            size, size, -size, 0.0f, 0.0f,
            size, -size, -size, 0.0f, 0.0f,

            -size, -size, size, 0.0f, 0.0f,
            -size, size, size, 0.0f, 0.0f,
            size, size, size, 0.0f, 0.0f,
            +size, -size, size, 0.0f, 0.0f,
        };

        ID3D11Buffer* VertexBuffer;
        unsigned int vertexStride = 5 * sizeof(float);
        unsigned int vertexOffset = 0;
        unsigned int vertexCount = 8;

        D3D11_BUFFER_DESC vertexDescription;
        ZeroMemory(&vertexDescription, sizeof(vertexDescription));
        vertexDescription.Usage = D3D11_USAGE_DEFAULT;
        vertexDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertexDescription.ByteWidth = sizeof(vertex_data_array);

        D3D11_SUBRESOURCE_DATA resourceData;
        ZeroMemory(&resourceData, sizeof(resourceData));
        resourceData.pSysMem = vertex_data_array;

        unsigned int indices[] =
        {
            // Front face
            0, 1, 2,
            0, 2, 3,
            // Back face
            4, 6, 5,
            4, 7, 6,
            // Left face
            4, 5, 1,
            4, 1, 0,
            // Right face
            3, 2, 6,
            3, 6, 7,
            // Top face
            1, 5, 6,
            1, 6, 2,
            // Bottom face
            4, 0, 3,
            4, 3, 7,
        };
        unsigned int indexCount = 36;


        ID3D11Buffer* IndexBuffer;
        D3D11_BUFFER_DESC indexDescription;
        ZeroMemory(&indexDescription, sizeof(indexDescription));
        indexDescription.Usage = D3D11_USAGE_DEFAULT;
        indexDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
        indexDescription.ByteWidth = sizeof(indices);

        D3D11_SUBRESOURCE_DATA indexResourceData;
        ZeroMemory(&indexResourceData, sizeof(indexResourceData));
        indexResourceData.pSysMem = indices;

        if (SUCCEEDED(Device->CreateBuffer(&vertexDescription, &resourceData, &VertexBuffer)) && SUCCEEDED(
            Device->CreateBuffer(&indexDescription, &indexResourceData, &IndexBuffer)))
        {
            Result = std::make_shared<DirectX11Mesh>(Context, VertexBuffer, IndexBuffer, vertexStride,
                                                     vertexOffset,
                                                     vertexCount, indexCount);
            Renderables[ShaderIn].push_back(Result);
        }
    }

    return Result;
}

std::shared_ptr<IRenderable> DirectX11Graphics::CreateMeshRenderable(IShader* ShaderIn, Mesh* mesh)
{
    std::shared_ptr<IRenderable> Result = nullptr;

    if (IsValid() && mesh)
    {
        ID3D11Buffer* VertexBuffer;
        unsigned int vertexStride = sizeof(Vertex);
        unsigned int vertexOffset = 0;
        unsigned int vertexCount = static_cast<unsigned int>(mesh->Vertices.size());

        D3D11_BUFFER_DESC vertexDescription;
        ZeroMemory(&vertexDescription, sizeof(vertexDescription));
        vertexDescription.Usage = D3D11_USAGE_DEFAULT;
        vertexDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertexDescription.ByteWidth = static_cast<UINT>(vertexCount * sizeof(Vertex));

        D3D11_SUBRESOURCE_DATA resourceData;
        ZeroMemory(&resourceData, sizeof(resourceData));
        resourceData.pSysMem = mesh->Vertices.data();

        unsigned int indexCount = static_cast<unsigned int>(mesh->Indices.size());

        ID3D11Buffer* IndexBuffer;
        D3D11_BUFFER_DESC indexDescription;
        ZeroMemory(&indexDescription, sizeof(indexDescription));
        indexDescription.Usage = D3D11_USAGE_DEFAULT;
        indexDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
        indexDescription.ByteWidth = static_cast<UINT>(indexCount * sizeof(unsigned int));

        D3D11_SUBRESOURCE_DATA indexResourceData;
        ZeroMemory(&indexResourceData, sizeof(indexResourceData));
        indexResourceData.pSysMem = mesh->Indices.data();

        if (SUCCEEDED(Device->CreateBuffer(&vertexDescription, &resourceData, &VertexBuffer)) &&
            SUCCEEDED(Device->CreateBuffer(&indexDescription, &indexResourceData, &IndexBuffer)))
        {
            Result = std::make_shared<DirectX11Mesh>(Context, VertexBuffer, IndexBuffer, vertexStride,
                                                     vertexOffset, vertexCount, indexCount);
            Renderables[ShaderIn].push_back(Result);
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
            camera->SetWidth(texWidth);
            camera->SetHeight(texHeight);
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

void DirectX11Graphics::SetWorldMatrix(const std::weak_ptr<Transform3D> transform)
{
    if (std::shared_ptr<Transform3D> trans = transform.lock())
    {
        DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(trans->Position.X(), trans->Position.Y(),
                                                                     trans->Position.Z());
        DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYaw(
            trans->Rotation.X(), trans->Rotation.Y(),
            trans->Rotation.Z());
        DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(trans->Scale.X(), trans->Scale.Y(),
                                                           trans->Scale.Z());
        DirectX::XMMATRIX world = scale * rotation * translation;
        DirectX::XMMATRIX mvp = DirectX::XMMatrixMultiply(world, vpMatrix);
        mvp = DirectX::XMMatrixTranspose(mvp);
        Context->UpdateSubresource(Mvp, 0, 0, &mvp, 0, 0);
        Context->VSSetConstantBuffers(0, 1, &Mvp);
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
