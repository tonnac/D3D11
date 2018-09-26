#pragma once
#include "Object.h"
#include "Terrain.h"
#include "Background.h"
#include "KahoAttack.h"
#include "Player.h"

class ObjectMgr : public Singleton<ObjectMgr>
{
	friend class Singleton<ObjectMgr>;
	using P_Effect = std::vector<PlayerEffectPtr>;
	using P_EffectIter = P_Effect::iterator;
private:
	ObjectMgr();
public:
	bool						Frame();
	bool						Render(ID3D11DeviceContext* pContext);
	bool						Release();
public:
	void						AddBackGround(Background* pBackGround);
	void						AddTerrain(Terrain* pTerrain);
	void						AddPlayerEffect(PlayerEffectPtr pEffect);
	void						AddPlayer(Player* pPlayer);
private:
	Player *					m_pPlayer;
	Background*					m_pBackground;
	std::vector<Terrain*>		m_Terrainlist;
	P_Effect					m_PlayerEffect;
};

#define S_Object ObjectMgr::getInst()