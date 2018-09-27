#include "ObjectMgr.h"
#include "DirectInput.h"

ObjectMgr::ObjectMgr()
{
	m_PlayerEffect.clear();
	m_Terrainlist.clear();
}

bool ObjectMgr::Frame()
{
	if (S_Input.getKeyState(DIK_RIGHT) == Input::KEYSTATE::KEY_HOLD)
	{
		if (m_pBackground->Scroll(g_fSecPerFrame * 200.0f) == true)
		{
			for (auto& it : m_Terrainlist)
			{
				it->Scroll(g_fSecPerFrame * 600.0f);
			}
		}
	}
	if (S_Input.getKeyState(DIK_LEFT) == Input::KEYSTATE::KEY_HOLD)
	{
		if (m_pBackground->Scroll(-g_fSecPerFrame * 200.0f) == true)
		{
			for (auto& it : m_Terrainlist)
			{
				it->Scroll(-g_fSecPerFrame * 600.0f);
			}
		}
	}
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
	m_pBackground->Frame();
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
	m_pPlayer->Frame();
	return true;
}
bool ObjectMgr::Render(ID3D11DeviceContext* pContext)
{
//	m_pBackground->Render(pContext);
	if (g_DebugMode == true)
	{
		for (auto& it : m_Terrainlist)
		{
			it->Render(pContext);
		}
	}
	m_pPlayer->Render(pContext);
	if (m_PlayerEffect.empty() == false)
	{
		for (auto& it : m_PlayerEffect)
		{
			it->Render(pContext);
		}
	}
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