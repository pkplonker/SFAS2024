#pragma once
#include <memory>
#include <PrimitiveBatch.h>
#include "effects.h"
#include <CommonStates.h>
#include <DirectXCollision.h>
#include <DirectXColors.h>
#include <vector>

#include "IRenderStrategy.h"

class ICamera;

namespace DirectX
{
    namespace DX11
    {
        class BasicEffect;
        struct VertexPositionColor;
    }

    struct BoundingBox;
}

struct BoundingBoxDrawData
{
    DirectX::BoundingBox bounds;
    DirectX::XMVECTORF32 color;
};

class DebugDrawer : public IRenderStrategy
{
public:
    static void DrawBoundingBox(const DirectX::BoundingBox& bounds,
                                DirectX::XMVECTORF32 color = DirectX::Colors::White);
    static void DrawGrid(bool state);
    void Update() override;
    DebugDrawer(std::weak_ptr<ICamera> weakCam);
    ~DebugDrawer() override;

private:
    void DrawGridInternal();
    void CreateInputLayout(ID3D11Device* device, DirectX::BasicEffect* effect);

    static inline std::unique_ptr<DirectX::PrimitiveBatch<DirectX::DX11::VertexPositionColor>> lineBatch;
    std::unique_ptr<DirectX::BasicEffect> effect{};
    ID3D11InputLayout* inputLayout{};
    ID3D11DeviceContext* context;
    ID3D11Device* device;
    std::weak_ptr<ICamera> camera;
    std::unique_ptr<DirectX::CommonStates> states{};
    static inline bool drawGrid;
    static inline bool init;
    static inline std::vector<BoundingBoxDrawData> drawData;
};
