#include "SceneMgr.h"
//#include "mSound.h"

SceneMgr::SceneMgr()
{
}

bool SceneMgr::Init()
{
	return true;
}
bool SceneMgr::Frame()
{

	return true;
}
bool SceneMgr::Render()
{
	m_pCurrentScene->Render();
	return true;
}
bool SceneMgr::Release()
{
	m_pCurrentScene->Release();
	delete m_pCurrentScene;
	return true;
}