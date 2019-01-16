#pragma once

#include "ObjectStd.h"
#include "GeometryStroage.h"
#include "RenderItemStorage.h"
#include "GeometryGenerator.h"

template<typename X>
class Object
{
public:
	Object(ID3D11Device* device) 
		: m_pDevice(device)
	{};
	Object() = default;
	virtual ~Object() = default;

public:
	virtual bool Frame() = 0;
	virtual bool Render(ID3D11DeviceContext* pContext) = 0;

	virtual void SetWorld(DirectX::FXMMATRIX world) = 0;
	virtual void SetWorld(const DirectX::XMFLOAT4X4& world) = 0;

	virtual bool Intersects(DirectX::FXMVECTOR& origin, DirectX::FXMVECTOR& dir, DirectX::CXMMATRIX& invView, float& tmin) = 0;

protected:
	virtual void BuildVBIB(LPVOID vertices, LPVOID indices, const UINT vbByteSize, const UINT ibByteSize, UINT vertexStride = sizeof(Vertex))
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

protected:
	ID3D11Device* m_pDevice = nullptr;

	MeshGeometry* mGeometry = nullptr;

	DirectX::XMFLOAT3 m_vPosition;
	DirectX::XMFLOAT3 m_vLook;
	DirectX::XMFLOAT3 m_vSide;
	DirectX::XMFLOAT3 m_vUp;

	DirectX::BoundingBox mBoundingBox;
};