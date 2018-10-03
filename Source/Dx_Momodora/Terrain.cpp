#include "Terrain.h"
#include "DirectInput.h"
#include "DirectWrite.h"
#include "Player.h"

bool Terrain::InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& ShaderFilepath,
			const std::string& VSFunc, const std::string& PSFunc)
{
	m_ConstantData.Color.x = 0.0f;
	m_ConstantData.Color.y = 0.0f;
	m_ConstantData.Color.z = 1.0f;
	m_ConstantData.Color.w = 1.0f;

	CreateVertexBuffer(pDevice);
	m_Object.CreateShader(pDevice, Name, ShaderFilepath, VSFunc, PSFunc);
	m_Object.CreateRasterizer(pDevice);
	CreateIndexBuffer(pDevice);
	CreateConstantBuffer(pDevice);
	Init();
	return true;
}
void Terrain::SetPos(const D3DXVECTOR4& Pos)
{
	D3DXVECTOR4 DrawVec = Pos;
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
COL Terrain::Collision(std::shared_ptr<Object> pObject, FLOAT* ColSize)
{
	FLOAT Size;
	COL col = Plane_Object::Collision(pObject, &Size);
	switch (col)
	{
	case COL::LEFT:
	{
		pObject->MoveCenterPos({ -Size, 0.0f });
	}break;
	case COL::TOP:
	{
		pObject->MoveCenterPos({ 0.0f, -Size });
	}break;
	case COL::RIGHT:
	{
		pObject->MoveCenterPos({ Size, 0.0f });
	}break;
	case COL::BOTTOM:
	{
		pObject->MoveCenterPos({ 0.0f, Size + 45.0f });
	}break;
	}
	return col;
}
//COL	Terrain::Collision(std::shared_ptr<Character> pObject)
//{
//	FLOAT Size;
//	COL col = Collision(pObject, &Size);
//
//	if (col == COL::TOP)
//	{
//		return col;
//	}
//
//	D2D1_RECT_F pcol = pObject->getCollisionRT();
//	D2D1_RECT_F dCol = { pcol.left, pcol.bottom, pcol.right, pcol.bottom + 15.0f };
//	D3DXVECTOR2 dColcen = { (dCol.right + dCol.left) * 0.5f, (dCol.bottom + dCol.top) * 0.5f };
//
//
//	if (pObject->getCurrentState() != L"Fall" &&
//		(dCol.right - dColcen.x) + m_rtCollision.right - m_Centerpos.x >= abs(m_Centerpos.x - dColcen.x) && 
//		(dCol.bottom - dColcen.y) + m_rtCollision.bottom- m_Centerpos.y >= abs(m_Centerpos.y - dColcen.y))
//	{
//		return COL::TOP;
//	}
//	else
//	{
//		return COL::NONE;
//	}
//}
COL	Terrain::Collision(std::shared_ptr<Player> pPlayer)
{
	FLOAT Size;
	COL col = Collision(pPlayer, &Size);

	if (col == COL::TOP)
	{
		return col;
	}
	if (col == COL::BOTTOM)
	{
		pPlayer->setState(L"Fall");
		return col;
	}

	D2D1_RECT_F pcol = pPlayer->getCollisionRT();
	D2D1_RECT_F dCol = { pcol.left, pcol.bottom, pcol.right, pcol.bottom + 15.0f };
	D3DXVECTOR2 dColcen = { (dCol.right + dCol.left) * 0.5f, (dCol.bottom + dCol.top) * 0.5f };


	if (pPlayer->getCurrentState() != L"Fall" &&
		(dCol.right - dColcen.x) + m_rtCollision.right - m_Centerpos.x >= abs(m_Centerpos.x - dColcen.x) &&
		(dCol.bottom - dColcen.y) + m_rtCollision.bottom - m_Centerpos.y >= abs(m_Centerpos.y - dColcen.y))
	{
		return COL::TOP;
	}
	else
	{
		return COL::NONE;
	}
}
bool Terrain::PreRender(ID3D11DeviceContext* pContext)
{
	Plane_Object::PreRender(pContext);
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	return true;
}