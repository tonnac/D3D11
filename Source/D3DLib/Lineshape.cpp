#include "Shape.h"
#include <DirectXColors.h>

using namespace DirectX;

LineShape::LineShape()
{
	m_Primitive = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
}

void LineShape::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	d3dUtil::CreateInputLayout(m_pDevice, (DWORD)m_DxObject.m_pVSBlob.Get()->GetBufferSize(),
		m_DxObject.m_pVSBlob.Get()->GetBufferPointer(), layout, (UINT)std::size(layout), 
		m_DxObject.m_pInputLayout.GetAddressOf());
}

void LineShape::CreateVertexData()
{
	m_LineVertexList =
	{
		PC_VERTEX({XMFLOAT3(0.0f,0.0f,0.0f), XMFLOAT4(DirectX::Colors::Red)}),
		PC_VERTEX({XMFLOAT3(1.0f,0.0f,0.0f), XMFLOAT4(DirectX::Colors::Red)})
	};

	m_DxObject.m_iVertexSize = sizeof(PC_VERTEX);
	m_DxObject.m_iNumVertex = (UINT)std::size(m_LineVertexList);
}

void LineShape::CreateIndexData()
{
	m_IndexList =
	{ 0, 1 };

	m_DxObject.m_iNumIndex = (UINT)std::size(m_IndexList);
	m_DxObject.m_iIndexSize = sizeof(DWORD);
}

void LineShape::CreateVertexBuffer()
{
	d3dUtil::CreateVertexBuffer(m_pDevice,
		(UINT)m_LineVertexList.size(),
		m_DxObject.m_iVertexSize,
		m_LineVertexList.data(),
		m_DxObject.m_pVertexBuffer.GetAddressOf());
}

void LineShape::LoadVertexShader(std::tstring szName)
{
	d3dUtil::LoadVertexShaderFile(m_pDevice, szName.c_str(), m_DxObject.m_pVertexShader.GetAddressOf()
		, "VS_PC", m_DxObject.m_pVSBlob.GetAddressOf());
}

void LineShape::LoadPixelShader(std::tstring szName)
{
	d3dUtil::LoadPixelShaderFile(m_pDevice, szName.c_str(), m_DxObject.m_pPixelShader.GetAddressOf(), "PSLine");
}

bool LineShape::Draw(ID3D11DeviceContext* pContext, XMFLOAT3 vStart, XMFLOAT3 vEnd, XMFLOAT4 vColor)
{
	m_LineVertexList =
	{
		PC_VERTEX({XMFLOAT3(vStart), XMFLOAT4(vColor)}),
		PC_VERTEX({XMFLOAT3(vEnd), XMFLOAT4(vColor)})
	};

	pContext->UpdateSubresource(m_DxObject.m_pVertexBuffer.Get(), 0, nullptr, m_LineVertexList.data(), 0, 0);

	return Render(pContext);
}





DirectionShape::DirectionShape()
{
	m_Primitive = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
}

void DirectionShape::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	d3dUtil::CreateInputLayout(m_pDevice, (DWORD)m_DxObject.m_pVSBlob.Get()->GetBufferSize(),
		m_DxObject.m_pVSBlob.Get()->GetBufferPointer(), layout, (UINT)std::size(layout), m_DxObject.m_pInputLayout.GetAddressOf());
}

void DirectionShape::CreateVertexData()
{
	m_LineVertexList =
	{
		PC_VERTEX({XMFLOAT3(0.0f,0.0f,0.0f), XMFLOAT4(DirectX::Colors::Red)}),
		PC_VERTEX({XMFLOAT3(0.0f,0.0f,0.0f), XMFLOAT4(DirectX::Colors::Green)}),
		PC_VERTEX({XMFLOAT3(0.0f,0.0f,0.0f), XMFLOAT4(DirectX::Colors::Blue)}),
		PC_VERTEX({XMFLOAT3(1000.0f,0.0f,0.0f), XMFLOAT4(DirectX::Colors::Red)}),
		PC_VERTEX({XMFLOAT3(0.0f,1000.0f,0.0f), XMFLOAT4(DirectX::Colors::Green)}),
		PC_VERTEX({XMFLOAT3(0.0f,0.0f,1000.0f), XMFLOAT4(DirectX::Colors::Blue)}),
	};

	m_DxObject.m_iVertexSize = sizeof(PC_VERTEX);
	m_DxObject.m_iNumVertex = (UINT)std::size(m_LineVertexList);
}

void DirectionShape::CreateIndexData()
{
	m_IndexList =
	{ 0, 3, 1, 4, 2, 5 };

	m_DxObject.m_iNumIndex = (UINT)std::size(m_IndexList);
	m_DxObject.m_iIndexSize = sizeof(DWORD);
}

void DirectionShape::CreateVertexBuffer()
{
	d3dUtil::CreateVertexBuffer(m_pDevice,
		(UINT)m_LineVertexList.size(),
		m_DxObject.m_iVertexSize,
		m_LineVertexList.data(),
		m_DxObject.m_pVertexBuffer.GetAddressOf());
}

void DirectionShape::LoadVertexShader(std::tstring szName)
{
	d3dUtil::LoadVertexShaderFile(m_pDevice, szName.c_str(), m_DxObject.m_pVertexShader.GetAddressOf(),
		"VS_PC", m_DxObject.m_pVSBlob.GetAddressOf());
}

void DirectionShape::LoadPixelShader(std::tstring szName)
{
	d3dUtil::LoadPixelShaderFile(m_pDevice, szName.c_str(), m_DxObject.m_pPixelShader.GetAddressOf(), "PSLine");
}

