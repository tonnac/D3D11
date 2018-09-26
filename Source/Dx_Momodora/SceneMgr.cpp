#include "SceneMgr.h"
//#include "mSound.h"
#include "KahoAttack.h"

std::shared_ptr<PlayerEffect> g_Attack1 = nullptr;
std::shared_ptr<PlayerEffect> g_Attack2 = nullptr;
std::shared_ptr<PlayerEffect> g_Attack3 = nullptr;
std::shared_ptr<PlayerEffect> g_AirAttack = nullptr;

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

	g_Attack1 = std::make_shared<KahoAttack1>();
	g_Attack2 = std::make_shared<KahoAttack2>();
	g_Attack3 = std::make_shared<KahoAttack3>();
	g_AirAttack = std::make_shared<KahoAirAttack>();

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
	DEL_REL(m_pCurrentScene);
	return true;
}
void SceneMgr::InitArrow(PlayerEffectPtr Arrow)
{
	GameScene * pGameScene = dynamic_cast<GameScene*>(m_pCurrentScene);
	if (pGameScene != nullptr)
	{
		pGameScene->InitArrow(Arrow);
	}
}