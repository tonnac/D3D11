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

ObjectMgr::~ObjectMgr()
{
	Release();
}

bool ObjectMgr::Frame()
{
	ObjectFrame<PlayerPTR>(m_pPlayer);
	ObjectFrame<BackgroundPTR>(m_pBackground);
	Scroll();
	TerrainCollision();
	if (m_EnemyList.empty() == false)
	{
		for (auto & it : m_EnemyList)
		{
			TerrainCollision(it);
		}
	}
	ContainerFrame<TerrainList>(m_Terrainlist);
	ContainerFrame<EnemyList>(m_EnemyList);
	EffectFrame<P_Effect>(m_PlayerEffect);
	ContainerFrame<UIList>(m_UIList);
	return true;
}
bool ObjectMgr::Render(ID3D11DeviceContext* pContext)
{
	ObjectRender<BackgroundPTR>(pContext, m_pBackground);
	ContainerRender<EnemyList>(pContext, m_EnemyList);
	ObjectRender<PlayerPTR>(pContext, m_pPlayer);
	ContainerRender<P_Effect>(pContext, m_PlayerEffect);
	ContainerRender<UIList>(pContext, m_UIList);
	if (g_DebugMode == true)
	{
		ContainerRender<TerrainList>(pContext, m_Terrainlist);
	}
	return true;
}
bool ObjectMgr::Release()
{
	ContainerRelease<TerrainList>(m_Terrainlist);
	ContainerRelease<UIList>(m_UIList);
	ContainerRelease<EnemyList>(m_EnemyList);
	if (m_pBackground != nullptr)
	{
		m_pBackground->Release();
		m_pBackground.reset();
	}
	return true;
}
void ObjectMgr::AddUI(UIPTR pUI)
{
	m_UIList.push_back(pUI);
}
void ObjectMgr::AddBackGround(BackgroundPTR pBackGround)
{
	m_pBackground = pBackGround;
}
void ObjectMgr::AddTerrain(TerrainPTR pTerrain)
{
	m_Terrainlist.push_back(pTerrain);
}
void ObjectMgr::AddPlayerEffect(PlayerEffectPtr pEffect)
{
	m_PlayerEffect.push_back(pEffect);
}
void ObjectMgr::AddPlayer(PlayerPTR pPlayer)
{
	m_pPlayer = pPlayer;
}
void ObjectMgr::AddEnemy(EnemyPTR pEnemy)
{
	m_EnemyList.push_back(pEnemy);
}

void ObjectMgr::Scroll()
{
	D2D1_RECT_F Crt = m_pPlayer->getCollisionRT();
	D3DXVECTOR2 Cen = m_pPlayer->getCenterPos();
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
			m_pBackground->Scroll((Cen.x - m_fLeftFocus) * g_fSecPerFrame * m_fDiameter);
			for (auto& it : m_Terrainlist)
			{
				it->Scroll((Cen.x - m_fLeftFocus) * g_fSecPerFrame * m_fDiameter * 3.0f);
			}
			if (m_EnemyList.empty() == false)
			{
				for (auto & iter : m_EnemyList)
				{
					iter->Scroll((Cen.x - m_fLeftFocus) * g_fSecPerFrame * m_fDiameter * 3.0f);
				}
			}
			m_pPlayer->MoveCenterPos({ (m_fLeftFocus - Cen.x) * g_fSecPerFrame * m_fDiameter * 3.0f, 0.0f });
		}
		if (m_pPlayer->getDir() == -1 && m_pBackground->CanScrollLeft() == true)
		{
			m_pBackground->Scroll((Cen.x - m_fRightFocus) * g_fSecPerFrame * m_fDiameter);
			for (auto& it : m_Terrainlist)
			{
				it->Scroll((Cen.x - m_fRightFocus) * g_fSecPerFrame * m_fDiameter * 3.0f);
			}
			if (m_EnemyList.empty() == false)
			{
				for (auto & iter : m_EnemyList)
				{
					iter->Scroll((Cen.x - m_fRightFocus) * g_fSecPerFrame * m_fDiameter * 3.0f);
				}
			}
			m_pPlayer->MoveCenterPos({ (m_fRightFocus - Cen.x) * g_fSecPerFrame * m_fDiameter * 3.0f, 0.0f });
		}
	}
}
void ObjectMgr::TerrainCollision()
{
	if (m_Terrainlist.empty() == false)
	{
		COL type = COL::NONE;
		for (auto& it : m_Terrainlist)
		{
			type += it->Collision(m_pPlayer);
		}

		if (type == COL::NONE)
		{
			m_pPlayer->setLanding(false);
		}
		else if (type == COL::BOTTOM)
		{
			m_pPlayer->setLanding(false);
		}
		else
		{
			m_pPlayer->setLanding(true);
		}
	}
}
void ObjectMgr::TerrainCollision(EnemyPTR pEnemy)
{
	if (m_Terrainlist.empty() == false)
	{
		for (auto & it : m_Terrainlist)
		{
			it->Collision(pEnemy);
		}
	}
}