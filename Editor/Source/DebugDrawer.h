﻿#pragma once
#include <memory>
#include <PrimitiveBatch.h>
#include "effects.h"
#include <CommonStates.h>

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

class DebugDrawer : public IRenderStrategy
{
public:
    static void DrawBoundingBox(const DirectX::BoundingBox& bounds);
    void Update() override;
    DebugDrawer(std::weak_ptr<ICamera> weakCam);
    void CreateInputLayout(ID3D11Device* device, DirectX::BasicEffect* effect);
    ~DebugDrawer() override;

private:
    static inline std::unique_ptr<DirectX::PrimitiveBatch<DirectX::DX11::VertexPositionColor>> lineBatch;
    std::unique_ptr<DirectX::BasicEffect> effect;
    ID3D11InputLayout* inputLayout;
    ID3D11DeviceContext* context;
    ID3D11Device* device;
    std::weak_ptr<ICamera> camera;
    std::unique_ptr<DirectX::CommonStates> states;
};