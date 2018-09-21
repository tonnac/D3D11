#include "Terrain.h"

bool Terrain::InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& ShaderFilepath,
			const std::string& VSFunc, const std::string& PSFunc)
{
	CreateVertexBuffer(pDevice);
	m_Object.CreateShader(pDevice, Name, ShaderFilepath, VSFunc, PSFunc);
	m_Object.CreateRasterizer(pDevice);
	CreateIndexBuffer(pDevice);
	
	Init();
	return true;
}
void Terrain::SetPos(const FLOAT& rtLeft, const FLOAT& rtTop, const FLOAT& rtRight, const FLOAT& rtBottom)
{
	m_VertexList.resize(4);
	m_rtDraw.left = rtLeft;
	m_rtDraw.top = rtTop;
	m_rtDraw.right = rtRight;
	m_rtDraw.bottom = rtBottom;
	VertexUpdate();
}
void Terrain::SetPos(const D2D1_RECT_F& rt)
{
	m_VertexList.resize(4);
	m_rtDraw = rt;
	VertexUpdate();
}
void Terrain::VertexUpdate()
{
	D2D1_RECT_F vRt = ComputeRectCoord(m_rtDraw);
	m_VertexList[0].Pos = { vRt.left, vRt.top, 0.5f };
	m_VertexList[1].Pos = { vRt.right, vRt.top, 0.5f };
	m_VertexList[2].Pos = { vRt.left, vRt.bottom, 0.5f };
	m_VertexList[3].Pos = { vRt.right, vRt.bottom, 0.5f };
}

void Terrain::CreateIndexBuffer(ID3D11Device* pDevice)
{
	for (int i = 0; i < m_VertexList.size() / 4; ++i)
	{
		m_indiciesList.push_back(i * 4);
		m_indiciesList.push_back(i * 4 + 1);
		m_indiciesList.push_back(i * 4 + 3);
		m_indiciesList.push_back(i * 4 + 3);
		m_indiciesList.push_back(i * 4 + 2);
		m_indiciesList.push_back(i * 4);
	}
	m_Object.CreateBuffer(pDevice, D3D11_BIND_INDEX_BUFFER, &m_indiciesList.at(0), sizeof(DWORD) * CASTING(UINT, m_indiciesList.size()));
}
bool Terrain::PreRender(ID3D11DeviceContext* pContext)
{
	Plane_Object::PreRender(pContext);
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	return true;
}