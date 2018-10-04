#pragma once
#include "Object.h"
#include "LobbyMenu.h"
#include "Background.h"
#include "KahoAttack.h"
#include "Player.h"
#include "DownableObject.h"
#include "Ladder.h"
#include "Bar.h"
#include "Enemy.h"

class ObjectMgr : public Singleton<ObjectMgr>
{
	friend class Singleton<ObjectMgr>;
	using P_Effect = std::vector<PlayerEffectPtr>;
	using P_EffectIter = P_Effect::iterator;
	using TerrainList = std::vector<TerrainPTR>;
	using UIList = std::list<UIPTR>;
	using TerrainIter = TerrainList::iterator;
	using UIIter = UIList::iterator;
	using EnemyList = std::vector<EnemyPTR>;
private:
	ObjectMgr();
public:
	~ObjectMgr();
public:
	bool						Frame			();
	bool						Render			(ID3D11DeviceContext* pContext);
	bool						Release			();
public:
	void						AddUI			(UIPTR pUI);
	void						AddBackGround	(BackgroundPTR pBackground);
	void						AddTerrain		(TerrainPTR pTerrain);
	void						AddPlayerEffect	(PlayerEffectPtr pEffect);
	void						AddPlayer		(PlayerPTR pPlayer);
	void						AddEnemy		(EnemyPTR pEnemy);
private:
	template <typename X>
	void						ObjectFrame(X& pData)
	{
		if (pData != nullptr)
		{
			pData->Frame();
		}
	}
	template <typename X>
	void						ContainerFrame	(X& pData)
	{
		typename X::iterator iter;
		if (pData.empty() == false)
		{
			for (iter = pData.begin(); iter != pData.end();)
			{
				if ((*iter)->Frame() == false)
				{
					iter = pData.erase(iter);
				}
				else
				{
					++iter;
				}
			}
		}
	}
	template <typename X>
	void						EffectFrame(X& pData)
	{
		typename X::iterator iter;
		typename X::iterator deliter = pData.end();
		if (pData.empty() == false)
		{
			for (iter = pData.begin(); iter != pData.end();)
			{
				if (m_EnemyList.empty() == false)
				{
					COL col = COL::NONE;
					KahoBowAttack* pAttack = dynamic_cast<KahoBowAttack*>(iter->get());
					for (auto &it : m_EnemyList)
					{
						if (it->getCurrentState() != L"Hit")
						{
							col = (*iter)->Collision(it);
						}
						if (col > COL::NONE && pAttack != nullptr)
						{
							it->setTransition(E_EVENT::BEARROWATTACKED);
							deliter = iter;
						}
					}
				}

				if ((*iter)->Frame() == false)
				{
					iter = pData.erase(iter);
					deliter = pData.end();
				}
				else
				{
					++iter;
				}
			}
			if (deliter != pData.end())
			{
				pData.erase(deliter);
			}
		}
	}
	void						Scroll			();
	void						TerrainCollision();
	void						TerrainCollision(EnemyPTR pEnemy);
private:
	template <typename K>
	void						ObjectRender	(ID3D11DeviceContext* pContext, K& pData)
	{
		if (pData != nullptr)
		{
			pData->Render(pContext);
		}
	}
	template <typename X>
	void						ContainerRender	(ID3D11DeviceContext* pContext, X& pData)
	{
		if (pData.empty() == false)
		{
			for (auto &it : pData)
			{
				it->Render(pContext);
			}
		}
	}
private:
	template <typename X>
	void						ContainerRelease(X& pData)
	{
		typename X::iterator iter;
		for (iter = pData.begin(); iter != pData.end();)
		{
			iter = pData.erase(iter);
		}
	}
private:
	D2D1_RECT_F					m_Collision;
	PlayerPTR					m_pPlayer;
	BackgroundPTR				m_pBackground;
	TerrainList					m_Terrainlist;
	UIList						m_UIList;
	EnemyList					m_EnemyList;
	P_Effect					m_PlayerEffect;
private:
	const FLOAT					m_fLeftFocus;
	const FLOAT					m_fRightFocus;
	const FLOAT					m_fDiameter;
};

#define S_Object ObjectMgr::getInst()