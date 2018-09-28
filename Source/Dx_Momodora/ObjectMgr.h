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
	bool						Frame			();
	bool						Render			(ID3D11DeviceContext* pContext);
	bool						Release			();
public:
	void						AddBackGround	(Background* pBackGround);
	void						AddTerrain		(Terrain* pTerrain);
	void						AddPlayerEffect	(PlayerEffectPtr pEffect);
	void						AddPlayer		(Player* pPlayer);
private:
	void						Scroll			();
	void						EffectFrame		();
	void						TerrainFrame	();
private:
	void						EffectRender	(ID3D11DeviceContext* pContext);
	void						TerrainRender	(ID3D11DeviceContext* pContext);
private:
	D2D1_RECT_F					m_Collision;
	Player *					m_pPlayer;
	Background*					m_pBackground;
	std::vector<Terrain*>		m_Terrainlist;
	P_Effect					m_PlayerEffect;
private:
	const FLOAT					m_fLeftFocus;
	const FLOAT					m_fRightFocus;
	const FLOAT					m_fDiameter;
};

#define S_Object ObjectMgr::getInst()