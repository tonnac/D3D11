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
private:
	Scene *			m_pCurrentScene;
};

#define S_SceneMgr SceneMgr::getInstance()