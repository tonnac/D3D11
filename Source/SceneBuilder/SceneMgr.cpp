#include "SceneMgr.h"
//#include "mSound.h"

SceneMgr::SceneMgr()
{
}

bool SceneMgr::Init()
{
	return true;
}
void SceneMgr::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;
	m_pCurrentScene = new GameScene1;
	m_pCurrentScene->setDevice(pDevice, pContext);
	m_pCurrentScene->Init();
}
bool SceneMgr::Frame()
{
	m_pCurrentScene->Frame();
	return true;
}
bool SceneMgr::Render()
{
	m_pCurrentScene->Render();
	return true;
}
bool SceneMgr::Release()
{
	RELEASE(m_pCurrentScene);
	return true;
}