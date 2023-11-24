#pragma once

#include "Engine/IRenderable.h"
#include <d3d11.h>

class DirectX11Graphics;

class DirectX11Mesh : public IRenderable
{
public:
    DirectX11Mesh(ID3D11DeviceContext* ContextIn, ID3D11Buffer* VertexBufferIn, ID3D11Buffer* IndexBufferIn,unsigned int vertexStride,
                  unsigned int vertexOffset, unsigned int vertexCountIn, unsigned int indexCountIn);
    virtual ~DirectX11Mesh();
    virtual void Update();

private:
    ID3D11DeviceContext* Context;
    ID3D11Buffer* VertexBuffer;
    unsigned int vertexStride;
    unsigned int vertexOffset;
    unsigned int vertexCount;
    ID3D11Buffer* IndexBuffer;
    unsigned indexCount;
};
