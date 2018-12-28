#pragma once
#include "GeometryStroage.h"
#include "RenderItemStorage.h"
#include "GeometryGenerator.h"

class Shape
{
public:
	Shape();
	virtual ~Shape() = default;

public:
	virtual void Create(ID3D11Device* pDevice, const std::tstring& textureFile = std::tstring(), const std::tstring& normalTex = std::tstring());

	bool Frame();

	virtual bool Render(ID3D11DeviceContext* pContext);

protected:

	virtual void BuildGeometry() { return; };
	virtual void BuildRenderItem(const std::tstring& textureFile);
	virtual void BuildMaterials(const std::tstring& textureFile, const std::tstring& normalTex) { return; };

	void BuildVBIB(LPVOID vertices, LPVOID indices, const UINT vbByteSize, const UINT ibByteSize, UINT vertexStride = sizeof(Vertex));

protected:
	ID3D11Device* m_pDevice = nullptr;

	RenderItem* mRenderItem = nullptr;
	MeshGeometry* mGeometry = nullptr;

	DirectX::XMFLOAT3 m_vPosition;
	DirectX::XMFLOAT3 m_vLook;
	DirectX::XMFLOAT3 m_vSide;
	DirectX::XMFLOAT3 m_vUp;

	UINT Stride = 0;
	UINT offset = 0;
};

class BoxShape : public Shape
{
public:
	BoxShape(bool isDice = false);
	virtual ~BoxShape() = default;

protected:
	virtual void BuildGeometry()override;
	virtual void BuildMaterials(const std::tstring& textureFile, const std::tstring& normalTex)override;
private:
	bool mIsDice = false;
};

class LineShape : public Shape
{
public:
	LineShape();
	virtual ~LineShape() = default;
public:
	virtual void	BuildRenderItem(const std::tstring& textureFile)override;
	virtual void	BuildGeometry()override;
	bool			Draw(ID3D11DeviceContext* pContext, DirectX::XMFLOAT3 vStart, DirectX::XMFLOAT3 vEnd, DirectX::XMFLOAT4 vColor);

protected:
	std::array<VertexC, 2> m_LineVertexList;
};

class PlaneShape : public Shape
{
public:
	PlaneShape() = default;
	virtual ~PlaneShape() = default;

protected:
	virtual void BuildGeometry()override;
};

class DirectionShape : public Shape
{
public:
	DirectionShape();
	virtual ~DirectionShape() = default;

protected:
	virtual void BuildGeometry()override;
	virtual void BuildRenderItem(const std::tstring& textureFile)override;

protected:
	std::array<VertexC, 6> m_LineVertexList;
};

class SkyBox : public Shape
{
public:
	SkyBox() = default;
	~SkyBox() = default;

protected:
	virtual void BuildGeometry()override;
	virtual void BuildRenderItem(const std::tstring& textureFile)override;
	virtual void BuildMaterials(const std::tstring& textureFile, const std::tstring& normalTex)override;
};

class GridShape : public Shape
{
public:
	GridShape() = default;
	~GridShape() = default;

protected:
	virtual void BuildGeometry()override;
	virtual void BuildRenderItem(const std::tstring& textureFile)override;
	virtual void BuildMaterials(const std::tstring& textureFile, const std::tstring& normalTex)override;
};