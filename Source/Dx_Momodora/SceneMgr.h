#pragma once
#include "GameScene.h"

class SceneMgr : public Singleton<SceneMgr>
{
	friend class Singleton<SceneMgr>;
	SceneMgr();
public:
	bool			Init		();
	bool			Frame		();
	bool			Render		();
	bool			Release		();
	Scene*			CreateScene	(const INT&, const bool&);
private:
	INT				m_nIndex;
	Scene *			m_pCurrentScene;
};

#define S_SceneMgr SceneMgr::getInstance()