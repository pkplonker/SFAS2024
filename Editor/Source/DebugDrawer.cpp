#include "DebugDrawer.h"

#include <DirectXCollision.h>
#include <DirectXColors.h>
#include <DirectXMath.h>
#include <VertexTypes.h>

#include "IApplication.h"
#include "DirectX11/DirectX11Graphics.h"
#include "Logging/Debug.h"

void DebugDrawer::DrawBoundingBox(const DirectX::BoundingBox& bounds)
{
    DirectX::XMFLOAT3 corners[8];
    bounds.GetCorners(corners);

    DirectX::XMVECTOR vectorCorners[8];
    for (int i = 0; i < 8; ++i)
    {
        vectorCorners[i] = XMLoadFloat3(&corners[i]);
    }

    lineBatch->Begin();

    const int edges[12][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0},
        {4, 5}, {5, 6}, {6, 7}, {7, 4},
        {0, 4}, {1, 5}, {2, 6}, {3, 7}
    };

    for (int i = 0; i < 12; ++i)
    {
        int startIndex = edges[i][0];
        int endIndex = edges[i][1];
        lineBatch->DrawLine(DirectX::VertexPositionColor(vectorCorners[startIndex], DirectX::Colors::Red),
                            DirectX::VertexPositionColor(vectorCorners[endIndex], DirectX::Colors::Red));
    }

    lineBatch->End();
}

void DebugDrawer::Update()
{
    if (const auto& cam = camera.lock())
    {
        context->OMSetBlendState(states->Opaque(), nullptr, 0xFFFFFFFF);
        context->OMSetDepthStencilState(states->DepthNone(), 0);
        context->RSSetState(states->CullNone());
        effect->SetVertexColorEnabled(true);
        effect->SetView(cam->GetViewMatrix());
        effect->SetProjection(cam->GetProjectionMatrix());
        effect->Apply(context);
        
        context->IASetInputLayout(inputLayout);
        DirectX::BoundingBox bounds2 = DirectX::BoundingBox();
        bounds2.Center = DirectX::XMFLOAT3(0, 0, 0);
        bounds2.Extents = DirectX::XMFLOAT3(5, 5, 5);
        DrawBoundingBox(bounds2);
    }
}

void DebugDrawer::Init(const std::weak_ptr<EditorCamera>& weakCam)
{
    camera = weakCam;
    auto graphics = IApplication::GetGraphics();
    if (graphics)
    {
        auto dx11Graphics = dynamic_cast<DirectX11Graphics*>(graphics);
        if (dx11Graphics)
        {
            states = std::make_unique<DirectX::CommonStates>(dx11Graphics->GetDevice());

            context = dx11Graphics->GetContext();
            device = dx11Graphics->GetDevice();
            lineBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(context);
            effect = std::make_unique<DirectX::BasicEffect>(device);
            effect->SetVertexColorEnabled(true);
            if (const auto& cam = camera.lock())
            {
                effect->SetView(cam->GetViewMatrix());
                effect->SetProjection(cam->GetProjectionMatrix());
            }
            else
            {
                Error("Camera failed for debug drawing");
            }

            CreateInputLayout(device, effect.get());
        }
        else
        {
            Error("Failed to get graphics for debug drawing");
        }
    }
}

void DebugDrawer::CreateInputLayout(ID3D11Device* device, DirectX::BasicEffect* effect)
{
    void const* shaderByteCode;
    size_t byteCodeLength;

    effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

   HRESULT hr =  device->CreateInputLayout(DirectX::VertexPositionColor::InputElements,
                              DirectX::VertexPositionColor::InputElementCount,
                              shaderByteCode, byteCodeLength,
                              &inputLayout);
    if (FAILED(hr)) {
        Error("Failed to create debug input layout")
    }
}
