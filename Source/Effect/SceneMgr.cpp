#include "SceneMgr.h"
//#include "mSound.h"
#include "KahoAttack.h"

Effect*	g_Attack1 = nullptr;
Effect*	g_Attack2 = nullptr;
Effect*	g_Attack3 = nullptr;
Effect*	g_AirAttack = nullptr;

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

	g_Attack1 = new KahoAttack1;
	g_Attack2 = new KahoAttack2;
	g_Attack3 = new KahoAttack3;
	g_AirAttack = new KahoAirAttack;

	g_Attack1->InitSet(pDevice, L"Basic", Filepath::m_Pngpath[L"Kaho"], Filepath::m_Txtpath[L"Shader"]);
	g_Attack2->InitSet(pDevice, L"Basic", Filepath::m_Pngpath[L"Kaho"], Filepath::m_Txtpath[L"Shader"]);
	g_Attack3->InitSet(pDevice, L"Basic", Filepath::m_Pngpath[L"Kaho"], Filepath::m_Txtpath[L"Shader"]);
	g_AirAttack->InitSet(pDevice, L"Basic", Filepath::m_Pngpath[L"Kaho"], Filepath::m_Txtpath[L"Shader"]);
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