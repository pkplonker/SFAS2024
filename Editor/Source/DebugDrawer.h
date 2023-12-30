#pragma once
#include <memory>
#include <PrimitiveBatch.h>

#include "EditorCamera.h"
#include "effects.h"
#include <CommonStates.h>

namespace DirectX
{
    namespace DX11
    {
        class BasicEffect;
        struct VertexPositionColor;
    }

    struct BoundingBox;
}

class DebugDrawer
{
public:
    static void DrawBoundingBox(const DirectX::BoundingBox& bounds);
    static void Update();
    static void CreateInputLayout(ID3D11Device* device, DirectX::BasicEffect* effect);
    static void Init(const std::weak_ptr<EditorCamera>& weakCam);

private:
    inline static std::unique_ptr<DirectX::PrimitiveBatch<DirectX::DX11::VertexPositionColor>> lineBatch;
    inline static std::unique_ptr<DirectX::BasicEffect> effect;
    inline static ID3D11InputLayout* inputLayout;
    inline static ID3D11DeviceContext* context;
    inline static ID3D11Device* device;
    inline static std::weak_ptr<EditorCamera> camera;
    inline static std::unique_ptr<DirectX::CommonStates> states;
};
