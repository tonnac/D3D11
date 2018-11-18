#pragma once
#include "ObjectStd.h"
#include "MathHelper.h"

struct SRTMatrix
{
	float		fSpin = 0.0f;
	float		fOrbit = 0.0f;
	DirectX::XMFLOAT4X4	matScale;
	DirectX::XMFLOAT4X4	matSpinRotation;
	DirectX::XMFLOAT4X4	matOrbitRotation;
	DirectX::XMFLOAT4X4	matTranslate;
	DirectX::XMFLOAT4X4	matWorld;
	SRTMatrix()
	{
		matScale = MathHelper::Identity4x4();
		matSpinRotation = MathHelper::Identity4x4();
		matOrbitRotation = MathHelper::Identity4x4();
		matTranslate = MathHelper::Identity4x4();
		matWorld = MathHelper::Identity4x4();
	}
};

class Shape
{
public:
	Shape();
	virtual ~Shape() = default;

public:
	void Create(ID3D11Device* pDevice, std::tstring szShaderName, std::tstring szTextureName = std::tstring());

	void SetMatrix(DirectX::XMFLOAT4X4* pWorld = nullptr, DirectX::XMFLOAT4X4* pView = nullptr, DirectX::XMFLOAT4X4* pProj = nullptr);
	void SetColor(DirectX::XMFLOAT4 vColor);

	bool Init();
	bool Frame();

	bool Render(ID3D11DeviceContext* pContext);
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


	bool PreRender(ID3D11DeviceContext* pContext);
	bool PostRender(ID3D11DeviceContext* pContext);

public:
	ID3D11Device* m_pDevice = nullptr;

	DxObj m_DxObject;

	std::vector<PNCT_VERTEX> m_VertexList;
	std::vector<DWORD>		 m_IndexList;

	CB_DATA m_cbData;

	DirectX::XMFLOAT4X4 m_matWorld;
	DirectX::XMFLOAT4X4 m_matView;
	DirectX::XMFLOAT4X4 m_matProj;

	SRTMatrix  m_SRTMat;

	DirectX::XMFLOAT3 m_vPosition;
	DirectX::XMFLOAT3 m_vLook;
	DirectX::XMFLOAT3 m_vSide;
	DirectX::XMFLOAT3 m_vUp;

	D3D_PRIMITIVE_TOPOLOGY m_Primitive;
};

class BoxShape : public Shape
{
public:
	BoxShape() = default;
	virtual ~BoxShape() = default;

public:
	void CreateVertexData()override;
	void CreateIndexData()override;
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

public:
	std::array<PC_VERTEX, 2> m_LineVertexList;
};

class PlaneShape : public Shape
{
public:
	PlaneShape() = default;
	virtual ~PlaneShape() = default;

public:
	void CreateVertexData()override;
	void CreateIndexData()override;
};

class DirectionShape : public Shape
{
public:
	DirectionShape();
	virtual ~DirectionShape() = default;

public:
	void	CreateInputLayout()override;
	void	CreateVertexData()override;
	void	CreateIndexData()override;
	void	CreateVertexBuffer()override;
	void	LoadVertexShader(std::tstring szName)override;
	void	LoadPixelShader(std::tstring szName)override;

public:
	std::array<PC_VERTEX, 6> m_LineVertexList;
};