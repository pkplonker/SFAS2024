#pragma once

#include "Engine/IRenderable.h"
#include <d3d11.h>

#include "IMeshRenderable.h"

class DirectX11Graphics;

class DirectX11Mesh : public IMeshRenderable
{
public:
	DirectX11Mesh(std::string path,DirectX::BoundingBox, ID3D11DeviceContext* ContextIn, ID3D11Buffer* VertexBufferIn, ID3D11Buffer* IndexBufferIn, unsigned int vertexStride,
		unsigned int vertexOffset, unsigned int vertexCountIn, unsigned int indexCountIn);
	virtual ~DirectX11Mesh();
	void Update() override;
	unsigned int GetVertsCount() override { return vertexCount; }
	unsigned int GetTriangleCount() override { return indexCount/3; }
	virtual DirectX::BoundingBox GetBounds() { return aabb; }
	
private:
	ID3D11DeviceContext* Context;
	ID3D11Buffer* VertexBuffer;
	unsigned int vertexStride;
	unsigned int vertexOffset;
	unsigned int vertexCount;
	ID3D11Buffer* IndexBuffer;
	unsigned int indexCount;
	DirectX::BoundingBox aabb;
};
