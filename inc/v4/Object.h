#pragma once

#include "ObjectStd.h"
#include "GeometryStroage.h"
#include "RenderItemStorage.h"
#include "GeometryGenerator.h"

class Object
{
public:
	Object() = default;
	virtual ~Object() = default;

public:
	virtual void Create(ID3D11Device* pDevice, const std::tstring& textureFile = std::tstring(), const std::tstring& normalTex = std::tstring());

	virtual bool Frame() = 0;
	virtual bool Render(ID3D11DeviceContext* pContext) = 0;

	virtual void SetWorld(DirectX::FXMMATRIX world) = 0;
	virtual void SetWorld(const DirectX::XMFLOAT4X4& world) = 0;

	virtual bool Intersects(DirectX::FXMVECTOR& origin, DirectX::FXMVECTOR& dir, DirectX::CXMMATRIX& invView, float& tmin) = 0;

protected:
	virtual void BuildGeometry() = 0;
	virtual void BuildRenderItem() = 0;
	virtual void BuildMaterials(const std::tstring& textureFile, const std::tstring& normalTex) {};
	virtual void BuildMaterials() {};

	virtual void BuildVBIB(LPVOID vertices, LPVOID indices, const UINT vbByteSize, const UINT ibByteSize, UINT vertexStride = sizeof(Vertex));

protected:
	ID3D11Device* m_pDevice = nullptr;

	MeshGeometry* mGeometry = nullptr;

	DirectX::XMFLOAT3 m_vPosition;
	DirectX::XMFLOAT3 m_vLook;
	DirectX::XMFLOAT3 m_vSide;
	DirectX::XMFLOAT3 m_vUp;

	DirectX::BoundingBox mBoundingBox;
};