#include "Terrain.h"
#include "DirectInput.h"
#include "DirectWrite.h"

bool Terrain::InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& ShaderFilepath,
			const std::string& VSFunc, const std::string& PSFunc)
{
	CreateVertexBuffer(pDevice);
	m_Object.CreateShader(pDevice, Name, ShaderFilepath, VSFunc, PSFunc);
	m_Object.CreateRasterizer(pDevice);
	CreateIndexBuffer(pDevice);
	CreateConstantBuffer(pDevice);
	Init();
	return true;
}
void Terrain::SetPos(const D3DXVECTOR4& DrawVec)
{
	m_VertexList.resize(4);
	m_DrawVec = DrawVec;
	ConvertScreenCoord();
}
bool Terrain::Frame()
{
	m_rtCollision.left = m_VertexList[0].Pos.x;
	m_rtCollision.top = m_VertexList[0].Pos.y;
	m_rtCollision.right = m_VertexList[1].Pos.x;
	m_rtCollision.bottom = m_VertexList[2].Pos.y;
	return true;
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
void Terrain::Scroll(const FLOAT& pos)
{
	for (int i = 0; i < m_VertexList.size(); ++i)
	{
		m_VertexList[i].Pos.x += pos;
	}
}
void Terrain::ConvertScreenCoord()
{
	m_DrawVec.x *= g_rtClient.right / g_fImageWidth;
	m_DrawVec.y *= g_rtClient.bottom / g_fImageHeight;
	m_DrawVec.z *= g_rtClient.right / g_fImageWidth;
	m_DrawVec.w *= g_rtClient.bottom / g_fImageHeight;
	m_VertexList[0].Pos = D3DXVECTOR3(m_DrawVec.x, m_DrawVec.y, 0.5f);
	m_VertexList[1].Pos = D3DXVECTOR3(m_DrawVec.z, m_DrawVec.y, 0.5f);
	m_VertexList[2].Pos = D3DXVECTOR3(m_DrawVec.x, m_DrawVec.w, 0.5f);
	m_VertexList[3].Pos = D3DXVECTOR3(m_DrawVec.z, m_DrawVec.w, 0.5f);
}
