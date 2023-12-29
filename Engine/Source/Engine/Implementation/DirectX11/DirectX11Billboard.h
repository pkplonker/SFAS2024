#pragma once

#include "Engine/IRenderable.h"
#include <d3d11.h>
#include <DirectXMath.h>

class DirectX11Graphics;

class DirectX11Billboard : public IRenderable
{
public:
    DirectX11Billboard(ID3D11DeviceContext* ContextIn, ID3D11Buffer* VertexBufferIn, ID3D11Buffer* IndexBufferIn,
                       unsigned int vertexStride, unsigned int vertexOffset, unsigned int vertexCountIn,
                       unsigned int indexCount);
    virtual ~DirectX11Billboard();
    virtual void Update();
    unsigned int GetVertsCount() override { return 4; }
    unsigned int GetTriangleCount() override { return 2; }
    virtual DirectX::BoundingBox GetBounds() { return DirectX::BoundingBox(); }

private:
    ID3D11DeviceContext* Context;
    ID3D11Buffer* VertexBuffer;
    unsigned int vertexStride;
    unsigned int vertexOffset;
    unsigned int vertexCount;
    unsigned int indexCount;
    ID3D11Buffer* IndexBuffer;
};
