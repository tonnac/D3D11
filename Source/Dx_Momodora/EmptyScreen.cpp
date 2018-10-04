#include "EmptyScreen.h"


bool EmptyScreen::InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& ShaderFilepath,
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
bool EmptyScreen::Frame()
{
	return true;
}
void EmptyScreen::SetPos(const D3DXVECTOR4& Vec)
{
	m_VertexList[0].Pos = { Vec.x, Vec.y, 0.5f };
	m_VertexList[1].Pos = { Vec.z, Vec.y, 0.5f };
	m_VertexList[2].Pos = { Vec.x, Vec.w, 0.5f };
	m_VertexList[3].Pos = { Vec.z, Vec.w, 0.5f };
	m_Centerpos = { (Vec.x + Vec.z) * 0.5f, (Vec.y + Vec.w) * 0.5f };
}