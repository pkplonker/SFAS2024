#include "DirectX11Mesh.h"

DirectX11Mesh::DirectX11Mesh(ID3D11DeviceContext* ContextIn, ID3D11Buffer* VertexBufferIn,
                                       ID3D11Buffer* IndexBufferIn, unsigned int vertexStride,
                                       unsigned int vertexOffset, unsigned int vertexCountIn, unsigned int indexCount) :
    Context(ContextIn), VertexBuffer(VertexBufferIn), IndexBuffer(IndexBufferIn),vertexStride(vertexStride), vertexOffset(vertexOffset),
    vertexCount(vertexCountIn), indexCount(indexCount)
{
}

DirectX11Mesh::~DirectX11Mesh()
{
    if (VertexBuffer)
    {
        VertexBuffer->Release();
    }
}

void DirectX11Mesh::Update()
{
    if (Context)
    {
        Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        Context->IASetVertexBuffers(0, 1, &VertexBuffer, &vertexStride, &vertexOffset);
        Context->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
        Context->DrawIndexed(indexCount, 0, 0);
    }
}
