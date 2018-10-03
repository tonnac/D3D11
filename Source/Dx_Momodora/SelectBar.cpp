#include "SelectBar.h"


bool SelectBar::InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& ShaderFilepath,
	const std::string& VSFunc, const std::string& PSFunc)
{
	m_VertexList.resize(8);
	m_VertexList[0].Color = { 0.0f,0.0f,0.0f,0.0f };
	m_VertexList[1].Color = { 0.0f,0.0f,1.0f,0.0f };
	m_VertexList[2].Color = { 0.0f,0.0f,0.0f,0.0f };
	m_VertexList[3].Color = { 0.0f,0.0f,1.0f,0.0f };
	m_VertexList[4].Color = { 0.0f,0.0f,1.0f,0.0f };
	m_VertexList[5].Color = { 0.0f,0.0f,0.0f,0.0f };
	m_VertexList[6].Color = { 0.0f,0.0f,1.0f,0.0f };
	m_VertexList[7].Color = { 0.0f,0.0f,0.0f,0.0f };

	m_LengthDiff.x = 151.5f;
	m_LengthDiff.y = 21.0f;

	CreateVertexBuffer(pDevice);
	m_Object.CreateShader(pDevice, Name, ShaderFilepath, VSFunc, PSFunc);
	m_Object.CreateRasterizer(pDevice);
	CreateIndexBuffer(pDevice);
	CreateConstantBuffer(pDevice);
	return true;
}

bool SelectBar::Frame()
{
	m_ConstantData.Color.x = g_fGameTime;
	m_VertexList[0].Pos = { m_Centerpos.x - m_LengthDiff.x, m_Centerpos.y - m_LengthDiff.y, 0.5f };
	m_VertexList[1].Pos = { m_Centerpos.x, m_VertexList[0].Pos.y,0.5f };
	m_VertexList[2].Pos = { m_VertexList[0].Pos.x, m_Centerpos.y + m_LengthDiff.y, 0.5f };
	m_VertexList[3].Pos = { m_VertexList[1].Pos.x, m_VertexList[2].Pos.y, 0.5f };
	m_VertexList[4].Pos = m_VertexList[1].Pos;
	m_VertexList[5].Pos = { m_Centerpos.x + m_LengthDiff.x, m_VertexList[0].Pos.y, 0.5f };
	m_VertexList[6].Pos = m_VertexList[3].Pos;
	m_VertexList[7].Pos = { m_VertexList[5].Pos.x,m_VertexList[6].Pos.y, 0.5f };
	return true;
}