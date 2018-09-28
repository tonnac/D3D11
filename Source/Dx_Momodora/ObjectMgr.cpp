#include "ObjectMgr.h"
#include "DirectInput.h"

ObjectMgr::ObjectMgr() : m_fLeftFocus(g_rtClient.right / 4.0f + 50.0f), m_fRightFocus(g_rtClient.right / 4.0f * 3 - 50.0f), m_fDiameter(0.4f)
{
	m_PlayerEffect.clear();
	m_Terrainlist.clear();
	m_Collision.left = g_rtClient.right / 4.0f;
	m_Collision.top = 0.0f;
	m_Collision.right = g_rtClient.right / 4.0f * 3;
	m_Collision.bottom = CASTING(FLOAT, g_rtClient.bottom);
}

bool ObjectMgr::Frame()
{
	m_pPlayer->Frame();
	Scroll();
	TerrainFrame();
	m_pBackground->Frame();
	EffectFrame();
	return true;
}
bool ObjectMgr::Render(ID3D11DeviceContext* pContext)
{
	m_pBackground->Render(pContext);
	TerrainRender(pContext);
	m_pPlayer->Render(pContext);
	EffectRender(pContext);
	return true;
}
bool ObjectMgr::Release()
{
	for (auto& it : m_Terrainlist)
	{
		DEL_REL(it);
	}
	DEL_REL(m_pBackground);
	return true;
}
void ObjectMgr::AddBackGround(Background* pBackGround)
{
	m_pBackground = pBackGround;
}
void ObjectMgr::AddTerrain(Terrain* pTerrain)
{
	m_Terrainlist.push_back(pTerrain);
}
void ObjectMgr::AddPlayerEffect(PlayerEffectPtr pEffect)
{
	m_PlayerEffect.push_back(pEffect);
}
void ObjectMgr::AddPlayer(Player* pPlayer)
{
	m_pPlayer = pPlayer;
}

void ObjectMgr::Scroll()
{
	D2D1_RECT_F Crt = m_pPlayer->getCollisionRT();

	if (m_Collision.right < Crt.right)
	{
		if (m_pBackground->CanScrollRight() == true)
		{
			m_pBackground->Scroll((Crt.right - m_Collision.right) * m_fDiameter);
			for (auto& it : m_Terrainlist)
			{
				it->Scroll((Crt.right - m_Collision.right) * m_fDiameter * 3.0f);
			}
			m_pPlayer->MoveCenterPos({ (m_Collision.right - Crt.right) * m_fDiameter * 3.0f, 0.0f });
		}
	}
	else if (m_Collision.left > Crt.left)
	{
		if (m_pBackground->CanScrollLeft() == true)
		{
			m_pBackground->Scroll((Crt.left - m_Collision.left) * m_fDiameter);
			for (auto& it : m_Terrainlist)
			{
				it->Scroll((Crt.left - m_Collision.left) * m_fDiameter * 3.0f);
			}
			m_pPlayer->MoveCenterPos({ (m_Collision.left - Crt.left) * m_fDiameter * 3.0f, 0.0f });
		}
	}
	else
	{
		if (m_pPlayer->getDir() == 1 && m_pBackground->CanScrollRight() == true)
		{
			m_pBackground->Scroll((Crt.left - m_fLeftFocus) * g_fSecPerFrame * m_fDiameter);
			for (auto& it : m_Terrainlist)
			{
				it->Scroll((Crt.left - m_fLeftFocus) * g_fSecPerFrame * m_fDiameter * 3.0f);
			}
			m_pPlayer->MoveCenterPos({ (m_fLeftFocus - Crt.left) * g_fSecPerFrame * m_fDiameter * 3.0f, 0.0f });
		}
		if (m_pPlayer->getDir() == -1 && m_pBackground->CanScrollLeft() == true)
		{
			m_pBackground->Scroll((Crt.right - m_fRightFocus) * g_fSecPerFrame * m_fDiameter);
			for (auto& it : m_Terrainlist)
			{
				it->Scroll((Crt.right - m_fRightFocus) * g_fSecPerFrame * m_fDiameter * 3.0f);
			}
			m_pPlayer->MoveCenterPos({ (m_fRightFocus - Crt.right) * g_fSecPerFrame * m_fDiameter * 3.0f, 0.0f });
		}
	}
}
void ObjectMgr::TerrainFrame()
{
	COL type = COL::NONE;
	for (auto& it : m_Terrainlist)
	{
		type += it->Collision(m_pPlayer);
		it->Frame();
	}

	if (type == COL::NONE)
	{
		m_pPlayer->setLanding(false);
	}
	else
	{
		m_pPlayer->setLanding(true);
	}
}
void ObjectMgr::EffectFrame()
{
	if (m_PlayerEffect.empty() == false)
	{
		P_EffectIter iter;
		for (iter = m_PlayerEffect.begin(); iter != m_PlayerEffect.end();)
		{
			if ((*iter)->Frame() == false)
			{
				iter = m_PlayerEffect.erase(iter);
			}
			else
			{
				++iter;
			}
		}
	}
}
void ObjectMgr::EffectRender(ID3D11DeviceContext* pContext)
{
	if (m_PlayerEffect.empty() == false)
	{
		for (auto& it : m_PlayerEffect)
		{
			it->Render(pContext);
		}
	}
}
void ObjectMgr::TerrainRender(ID3D11DeviceContext* pContext)
{
	if (g_DebugMode == true)
	{
		for (auto& it : m_Terrainlist)
		{
			it->Render(pContext);
		}
	}
}