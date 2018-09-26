#pragma once
#include "GameScene.h"

class SceneMgr : public Singleton<SceneMgr>
{
	friend class Singleton<SceneMgr>;
	SceneMgr();
public:
	bool					Init		();
	void					Initialize	(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	bool					Frame		();
	bool					Render		();
	bool					Release		();
private:
	ID3D11Device *			m_pDevice;
	ID3D11DeviceContext *	m_pContext;
	Scene *					m_pCurrentScene;
};

#define S_Scene SceneMgr::getInst()