#pragma once
#include "Object.h"
#include "Terrain.h"
#include "Background.h"

class ObjectMgr : public Singleton<ObjectMgr>
{
	friend class Singleton<ObjectMgr>;
private:
	ObjectMgr();
public:
	bool					Frame();
	bool					Render(ID3D11DeviceContext* pContext);
	bool					Release();
public:
	void					AddBackGround(Background* pBackGround);
	void					AddTerrain(Terrain* pTerrain);
private:
	Background*				m_pBackground;
	std::vector<Terrain*>	m_Terrainlist;
	bool					isDebug;
};

#define S_Object ObjectMgr::getInst()