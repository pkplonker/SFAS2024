#include "DebugDrawer.h"

#include <DirectXCollision.h>
#include <DirectXMath.h>
#include <VertexTypes.h>

#include "IApplication.h"
#include "DirectX11/DirectX11Graphics.h"
#include "Logging/Debug.h"

void DebugDrawer::DrawBoundingBox(const DirectX::BoundingBox& bounds, DirectX::XMVECTORF32 color)
{
    if (!init)
    {
        Warning("Trying to draw volume before init");
        return;
    }

    drawData.push_back({bounds, color});
}

void DebugDrawer::Update()
{
    if (!init || drawData.empty()) return;

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
        
        lineBatch->Begin();

        for (const auto& data : drawData)
        {
            DirectX::XMFLOAT3 corners[8];
            data.bounds.GetCorners(corners);

            DirectX::XMVECTOR vectorCorners[8];
            for (int i = 0; i < 8; ++i)
            {
                vectorCorners[i] = XMLoadFloat3(&corners[i]);
            }

            constexpr int edges[12][2] = {
                {0, 1}, {1, 2}, {2, 3}, {3, 0},
                {4, 5}, {5, 6}, {6, 7}, {7, 4},
                {0, 4}, {1, 5}, {2, 6}, {3, 7}
            };

            for (const auto edge : edges)
            {
                const int startIndex = edge[0];
                const int endIndex = edge[1];
                lineBatch->DrawLine(DirectX::VertexPositionColor(vectorCorners[startIndex], data.color),
                                    DirectX::VertexPositionColor(vectorCorners[endIndex], data.color));
            }
        }

        lineBatch->End();
        drawData.clear();
    }
}

DebugDrawer::DebugDrawer(const std::weak_ptr<ICamera> weakCam) : camera(weakCam)
{
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
            init = true;
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

    HRESULT hr = device->CreateInputLayout(DirectX::VertexPositionColor::InputElements,
                                           DirectX::VertexPositionColor::InputElementCount,
                                           shaderByteCode, byteCodeLength,
                                           &inputLayout);
    if (FAILED(hr))
    {
        Error("Failed to create debug input layout")
    }
}

DebugDrawer::~DebugDrawer()
{
}
