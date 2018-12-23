#pragma once
#include "ObjectStd.h"
#include "MathHelper.h"

class Shape
{
public:
	Shape();
	virtual ~Shape() = default;

public:
	void Create(ID3D11Device* pDevice, std::tstring szShaderName, std::tstring szTextureName = std::tstring());

	void SetMatrix(DirectX::XMFLOAT4X4* pWorld = nullptr);

	bool Init();
	bool Frame();

	bool PreRender(ID3D11DeviceContext* pContext);
	bool PostRender(ID3D11DeviceContext* pContext);
	bool Render(ID3D11DeviceContext* pContext);

	virtual void SetShaderResourceView(ID3D11ShaderResourceView* pShaderResourceView);

protected:
	virtual void CreateVertexData() { return; };
	virtual void CreateIndexData() { return; };

	virtual void CreateVertexBuffer();
	virtual void CreateIndexBuffer();
	virtual void CreateConstantBuffer();
	virtual void CreateInputLayout();
	virtual void LoadVertexShader(std::tstring szName);
	virtual void LoadPixelShader(std::tstring szName);
	virtual void LoadGeometryShader(std::tstring szName);
	virtual void LoadTextureShader(std::tstring szName);

protected:
	ID3D11Device* m_pDevice = nullptr;

	DxObj m_DxObject;

	std::vector<Vertex> m_VertexList;
	std::vector<DWORD>		 m_IndexList;

	ObjectConstants m_cbData;

	DirectX::XMFLOAT4X4 m_matWorld = MathHelper::Identity4x4();

	DirectX::XMFLOAT3 m_vPosition;
	DirectX::XMFLOAT3 m_vLook;
	DirectX::XMFLOAT3 m_vSide;
	DirectX::XMFLOAT3 m_vUp;

	D3D_PRIMITIVE_TOPOLOGY m_Primitive;
};

class BoxShape : public Shape
{
public:
	BoxShape(bool isDice = false);
	virtual ~BoxShape() = default;

protected:
	void CreateVertexData()override;
	void CreateIndexData()override;

private:
	void DiceTex();

private:
	bool mIsDice = false;
};

class LineShape : public Shape
{
public:
	LineShape();
	virtual ~LineShape() = default;
public:
	void	CreateInputLayout()override;
	void	CreateVertexData()override;
	void	CreateIndexData()override;
	void	CreateVertexBuffer()override;
	void	LoadVertexShader(std::tstring szName)override;
	void	LoadPixelShader(std::tstring szName)override;
	bool	Draw(ID3D11DeviceContext* pContext, DirectX::XMFLOAT3 vStart, DirectX::XMFLOAT3 vEnd, DirectX::XMFLOAT4 vColor);

protected:
	std::array<VertexC, 2> m_LineVertexList;
};

class PlaneShape : public Shape
{
public:
	PlaneShape() = default;
	virtual ~PlaneShape() = default;

protected:
	void CreateVertexData()override;
	void CreateIndexData()override;
};

class DirectionShape : public Shape
{
public:
	DirectionShape();
	virtual ~DirectionShape() = default;

protected:
	void	CreateInputLayout()override;
	void	CreateVertexData()override;
	void	CreateIndexData()override;
	void	CreateVertexBuffer()override;
	void	LoadVertexShader(std::tstring szName)override;
	void	LoadPixelShader(std::tstring szName)override;

protected:
	std::array<VertexC, 6> m_LineVertexList;
};