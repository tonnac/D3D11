#pragma once
#include "GameScene.h"
#include "Fade.h"

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
public:
	void					InitArrow	(PlayerEffectPtr Arrow);
private:
	void					SceneChange	();
	Scene*					getScene	(const bool& isNextScene);
private:
	INT						m_iSceneIndex;
	INT						m_iCount;
	ID3D11Device *			m_pDevice;
	ID3D11DeviceContext *	m_pContext;
	Scene *					m_pCurrentScene;
	std::shared_ptr<Fade>	m_pFade;
};

#define S_Scene SceneMgr::getInst()