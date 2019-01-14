#include "Object.h"

using namespace DirectX;

void Object::Create(ID3D11Device* pDevice, const std::tstring& textureFile, const std::tstring& normalTex)
{
	m_pDevice = pDevice;

	BuildGeometry();
	BuildMaterials(textureFile, normalTex);
	BuildRenderItem();
}

void Object::BuildVBIB(LPVOID vertices, LPVOID indices, const UINT vbByteSize, const UINT ibByteSize, UINT vertexStride)
{
	D3DCreateBlob(vbByteSize, mGeometry->VertexBufferCPU.GetAddressOf());
	CopyMemory(mGeometry->VertexBufferCPU->GetBufferPointer(), vertices, vbByteSize);

	D3DCreateBlob(ibByteSize, mGeometry->IndexBufferCPU.GetAddressOf());
	CopyMemory(mGeometry->IndexBufferCPU->GetBufferPointer(), indices, ibByteSize);

	mGeometry->VertexBufferByteSize = vbByteSize;
	mGeometry->VertexByteStride = vertexStride;
	mGeometry->IndexBufferByteSize = ibByteSize;
	mGeometry->IndexFormat = DXGI_FORMAT_R32_UINT;

	d3dUtil::CreateVertexBuffer(m_pDevice,
		mGeometry->VertexBufferByteSize,
		mGeometry->VertexBufferCPU->GetBufferPointer(),
		mGeometry->VertexBuffer.GetAddressOf());

	d3dUtil::CreateIndexBuffer(m_pDevice,
		mGeometry->IndexBufferByteSize,
		mGeometry->IndexBufferCPU->GetBufferPointer(),
		mGeometry->IndexBuffer.GetAddressOf());
}