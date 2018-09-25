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
void Terrain::SetPos(const D3DXVECTOR4& Drawvec)
{
	D3DXVECTOR4 DrawVec = Drawvec;
	DrawVec.x *= g_rtClient.right / g_fImageWidth;
	DrawVec.y *= g_rtClient.bottom / g_fImageHeight;
	DrawVec.z *= g_rtClient.right / g_fImageWidth;
	DrawVec.w *= g_rtClient.bottom / g_fImageHeight;
	m_LengthDiff.x = (DrawVec.z - DrawVec.x) * 0.5f;
	m_LengthDiff.y = (DrawVec.w - DrawVec.y) * 0.5f;
	m_VertexList[0].Pos = D3DXVECTOR3(DrawVec.x, DrawVec.y, 0.5f);
	m_VertexList[1].Pos = D3DXVECTOR3(DrawVec.z, DrawVec.y, 0.5f);
	m_VertexList[2].Pos = D3DXVECTOR3(DrawVec.x, DrawVec.w, 0.5f);
	m_VertexList[3].Pos = D3DXVECTOR3(DrawVec.z, DrawVec.w, 0.5f);
	m_Centerpos.x = (DrawVec.x + DrawVec.z) * 0.5f;
	m_Centerpos.y = (DrawVec.y + DrawVec.w) * 0.5f;
}
bool Terrain::Scroll(const FLOAT& pos)
{
	m_Centerpos.x += -pos;
	return true;
}
bool Terrain::PreRender(ID3D11DeviceContext* pContext)
{
	Plane_Object::PreRender(pContext);
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	return true;
}