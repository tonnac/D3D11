#include "Shape.h"
#include <DirectXColors.h>

using namespace DirectX;

Shape::Shape()
{
	m_matWorld = MathHelper::Identity4x4();
	m_matProj  = MathHelper::Identity4x4();
	m_matView  = MathHelper::Identity4x4();

	m_Primitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	
	m_vLook = { m_matWorld._11, m_matWorld._12 , m_matWorld._13 };
	m_vSide = { m_matWorld._21, m_matWorld._22 , m_matWorld._23 };
	m_vUp = { m_matWorld._31, m_matWorld._32 , m_matWorld._33 };
	m_vPosition = { m_matWorld._41, m_matWorld._42, m_matWorld._43 };
}

void Shape::Create(ID3D11Device* pDevice, std::tstring szShaderName, std::tstring szTextureName)
{
	m_pDevice = pDevice;
	CreateVertexData();
	CreateIndexData();
	CreateVertexBuffer();
	CreateIndexBuffer();
	CreateConstantBuffer();
	LoadVertexShader(szShaderName);
	LoadPixelShader(szShaderName);
//	LoadGeometryShader(szShaderName);
	CreateInputLayout();
	if (!szTextureName.empty())
	{
		LoadTextureShader(szTextureName);
	}
}

void Shape::CreateVertexBuffer()
{
	d3dUtil::CreateVertexBuffer(m_pDevice,
		(UINT)m_VertexList.size(),
		m_DxObject.m_iVertexSize,
		m_VertexList.data(),
		m_DxObject.m_pVertexBuffer.GetAddressOf());
}

void Shape::CreateIndexBuffer()
{
	d3dUtil::CreateIndexBuffer(m_pDevice, (UINT)m_IndexList.size(), m_DxObject.m_iIndexSize, m_IndexList.data(), m_DxObject.m_pIndexBuffer.GetAddressOf());
}

void Shape::CreateConstantBuffer()
{
	d3dUtil::CreateConstantBuffer(m_pDevice, 1, sizeof(CB_DATA), m_DxObject.m_pConstantBuffer.GetAddressOf());
}

void Shape::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION"	, 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, 0	, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL"	, 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, 12	, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR"	, 0, DXGI_FORMAT_R32G32B32A32_FLOAT	, 0, 24	, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD"	, 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, 40	, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	d3dUtil::CreateInputLayout(m_pDevice,
		(DWORD)m_DxObject.m_pVSBlob->GetBufferSize(),
		m_DxObject.m_pVSBlob->GetBufferPointer(),
		layout,
		(UINT)std::size(layout),
		m_DxObject.m_pInputLayout.GetAddressOf());
}

void Shape::LoadVertexShader(std::tstring szName)
{
	d3dUtil::LoadVertexShaderFile(m_pDevice, szName.c_str(), m_DxObject.m_pVertexShader.GetAddressOf(), "VS", m_DxObject.m_pVSBlob.GetAddressOf());
}

void Shape::LoadPixelShader(std::tstring szName)
{
	d3dUtil::LoadPixelShaderFile(m_pDevice, szName.c_str(), m_DxObject.m_pPixelShader.GetAddressOf());
}

void Shape::LoadGeometryShader(std::tstring szName)
{
	d3dUtil::LoadGeometryShaderFile(m_pDevice, szName.c_str(), m_DxObject.m_pGeometryShader.GetAddressOf());
}

void Shape::LoadTextureShader(std::tstring szName)
{
	d3dUtil::CreateShaderResourceView(m_pDevice, szName, m_DxObject.m_pTextureSRV.GetAddressOf());
}

void Shape::SetMatrix(XMFLOAT4X4* pWorld, XMFLOAT4X4* pView, XMFLOAT4X4* pProj)
{
	if (pWorld != nullptr)
	{
		m_matWorld = *pWorld;
	}
	if (pView != nullptr)
	{
		m_matView = *pView;
	}
	if (pProj != nullptr)
	{
		m_matProj = *pProj;
	}

	m_vLook = { m_matWorld._11, m_matWorld._12 , m_matWorld._13 };
	m_vSide = { m_matWorld._21, m_matWorld._22 , m_matWorld._23 };
	m_vUp = { m_matWorld._31, m_matWorld._32 , m_matWorld._33 };
	m_vPosition = { m_matWorld._41, m_matWorld._42, m_matWorld._43 };

	XMMATRIX world = XMLoadFloat4x4(&m_matWorld);
	XMMATRIX view = XMLoadFloat4x4(&m_matView);
	XMMATRIX proj = XMLoadFloat4x4(&m_matProj);

	XMStoreFloat4x4(&m_cbData.matWorld, XMMatrixTranspose(world));
	XMStoreFloat4x4(&m_cbData.matView, XMMatrixTranspose(view));
	XMStoreFloat4x4(&m_cbData.matProj, XMMatrixTranspose(proj));
}

void Shape::SetColor(DirectX::XMFLOAT4 vColor)
{
	m_cbData.vColor = vColor;
}

bool Shape::Init()
{
	return true;
}

bool Shape::Frame()
{
	return true;
}

bool Shape::PreRender(ID3D11DeviceContext* pContext)
{
	pContext->UpdateSubresource(m_DxObject.m_pConstantBuffer.Get(), 0, nullptr, &m_cbData, 0, 0);
	return m_DxObject.PreRender(pContext);
}

bool Shape::PostRender(ID3D11DeviceContext* pContext)
{
	pContext->IASetPrimitiveTopology(m_Primitive);
	return m_DxObject.PostRender(pContext);
}

bool Shape::Render(ID3D11DeviceContext* pContext)
{
	PreRender(pContext);
	PostRender(pContext);
	return true;
}


void BoxShape::CreateVertexData()
{
	m_VertexList.resize(24);
	m_VertexList[0] = PNCT_VERTEX(XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f));
	m_VertexList[1] = PNCT_VERTEX(XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f));
	m_VertexList[2] = PNCT_VERTEX(XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f));
	m_VertexList[3] = PNCT_VERTEX(XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f));
	// µÞ¸é
	m_VertexList[4] = PNCT_VERTEX(XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f));
	m_VertexList[5] = PNCT_VERTEX(XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f));
	m_VertexList[6] = PNCT_VERTEX(XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f));
	m_VertexList[7] = PNCT_VERTEX(XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f));

	// ¿À¸¥ÂÊ
	m_VertexList[8] = PNCT_VERTEX(XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f));
	m_VertexList[9] = PNCT_VERTEX(XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f));
	m_VertexList[10] = PNCT_VERTEX(XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f));
	m_VertexList[11] = PNCT_VERTEX(XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f));

	// ¿ÞÂÊ
	m_VertexList[12] = PNCT_VERTEX(XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f));
	m_VertexList[13] = PNCT_VERTEX(XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f));
	m_VertexList[14] = PNCT_VERTEX(XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f));
	m_VertexList[15] = PNCT_VERTEX(XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f));

	// À­¸é
	m_VertexList[16] = PNCT_VERTEX(XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 0.5f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f));
	m_VertexList[17] = PNCT_VERTEX(XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 0.5f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f));
	m_VertexList[18] = PNCT_VERTEX(XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 0.5f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f));
	m_VertexList[19] = PNCT_VERTEX(XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 0.5f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f));

	// ¾Æ·§¸é
	m_VertexList[20] = PNCT_VERTEX(XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f));
	m_VertexList[21] = PNCT_VERTEX(XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f));
	m_VertexList[22] = PNCT_VERTEX(XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f));
	m_VertexList[23] = PNCT_VERTEX(XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f));

	m_DxObject.m_iNumVertex = (UINT)m_VertexList.size();
	m_DxObject.m_iVertexSize = sizeof(PNCT_VERTEX);

}

void BoxShape::CreateIndexData()
{
	m_IndexList.resize(36);

	int iIndex = 0;
	m_IndexList[iIndex++] = 0; 	m_IndexList[iIndex++] = 1; 	m_IndexList[iIndex++] = 2; 	m_IndexList[iIndex++] = 0;	m_IndexList[iIndex++] = 2; 	m_IndexList[iIndex++] = 3;
	m_IndexList[iIndex++] = 4; 	m_IndexList[iIndex++] = 5; 	m_IndexList[iIndex++] = 6; 	m_IndexList[iIndex++] = 4;	m_IndexList[iIndex++] = 6; 	m_IndexList[iIndex++] = 7;
	m_IndexList[iIndex++] = 8; 	m_IndexList[iIndex++] = 9; 	m_IndexList[iIndex++] = 10; m_IndexList[iIndex++] = 8;	m_IndexList[iIndex++] = 10; m_IndexList[iIndex++] = 11;
	m_IndexList[iIndex++] = 12; m_IndexList[iIndex++] = 13; m_IndexList[iIndex++] = 14; m_IndexList[iIndex++] = 12;	m_IndexList[iIndex++] = 14; m_IndexList[iIndex++] = 15;
	m_IndexList[iIndex++] = 16; m_IndexList[iIndex++] = 17; m_IndexList[iIndex++] = 18; m_IndexList[iIndex++] = 16;	m_IndexList[iIndex++] = 18; m_IndexList[iIndex++] = 19;
	m_IndexList[iIndex++] = 20; m_IndexList[iIndex++] = 21; m_IndexList[iIndex++] = 22; m_IndexList[iIndex++] = 20;	m_IndexList[iIndex++] = 22; m_IndexList[iIndex++] = 23;

	m_DxObject.m_iNumIndex = (UINT)m_IndexList.size();
	m_DxObject.m_iIndexSize = (UINT)sizeof(DWORD);
}