#include "Shape.h"

using namespace DirectX;

bool PlaneShape::Render(ID3D11DeviceContext * pContext)
{
	UINT Offset = 0;
	pContext->IASetVertexBuffers(0, 1, mGeometry->VertexBuffer.GetAddressOf(), &mGeometry->VertexByteStride, &Offset);
	pContext->IASetIndexBuffer(mGeometry->IndexBuffer.Get(), mGeometry->IndexFormat, 0);

	pContext->IASetPrimitiveTopology(mRenderItem->PrimitiveType);
	pContext->VSSetConstantBuffers(1, 1, mRenderItem->ConstantBuffer.GetAddressOf());
	pContext->DrawIndexedInstanced(mRenderItem->IndexCount, 1, mRenderItem->StartIndexLocation, mRenderItem->BaseVertexLocation, 0);
	return true;
}

void PlaneShape::BuildGeometry()
{
	std::vector<Vertex> vertices;
	std::vector<DWORD> indices;

	vertices.resize(4);
	vertices[0] = Vertex(XMFLOAT3(-1.0f, +1.0f, +0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f));
	vertices[1] = Vertex(XMFLOAT3(+1.0f, +1.0f, +0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f));
	vertices[2] = Vertex(XMFLOAT3(+1.0f, -1.0f, +0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f));
	vertices[3] = Vertex(XMFLOAT3(-1.0f, -1.0f, +0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f));

	indices.resize(6);
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(DWORD);

	BuildVBIB(vertices.data(), indices.data(), vbByteSize, ibByteSize);

	SubmeshGeometry sub;
	sub.IndexCount = (UINT)indices.size();
	sub.BaseVertexLocation = 0;
	sub.StartIndexLocation = 0;
	mGeometry->DrawArgs[L"default"] = sub;
}