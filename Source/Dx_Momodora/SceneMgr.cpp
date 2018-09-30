#include "SceneMgr.h"
//#include "mSound.h"
#include "KahoAttack.h"

std::shared_ptr<PlayerEffect>	g_Attack1 = nullptr;
std::shared_ptr<PlayerEffect>	g_Attack2 = nullptr;
std::shared_ptr<PlayerEffect>	g_Attack3 = nullptr;
std::shared_ptr<PlayerEffect>	g_AirAttack = nullptr;
std::shared_ptr<Player>			g_Player = nullptr;
std::shared_ptr<Fade>			g_Fade = nullptr;
std::shared_ptr<Setting>		g_Setting = nullptr;

SceneMgr::SceneMgr() : m_iSceneIndex(0), m_iCount(0)
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

	g_Fade = std::make_shared<Fade>();
	g_Fade->InitSet(pDevice, L"Fade", Filepath::m_Txtpath[L"Shader"], "VS", "FadePS");

	g_Setting = std::make_shared<Setting>();
	g_Setting->InitSet(m_pDevice, L"Baisc", Filepath::m_Pngpath[L"Lobby"], Filepath::m_Txtpath[L"Shader"]);

#pragma region Player_Init
	g_Attack1 = std::make_shared<KahoAttack1>();
	g_Attack2 = std::make_shared<KahoAttack2>();
	g_Attack3 = std::make_shared<KahoAttack3>();
	g_AirAttack = std::make_shared<KahoAirAttack>();
	g_Player = std::make_shared<Player>();
	g_Player->InitSet(m_pDevice, L"Player", Filepath::m_Pngpath[L"Kaho"], Filepath::m_Txtpath[L"Shader"], "VS", "PlayerPS");

	g_Attack1->InitSet(pDevice, L"Basic", Filepath::m_Pngpath[L"Kaho"], Filepath::m_Txtpath[L"Shader"]);
	g_Attack2->InitSet(pDevice, L"Basic", Filepath::m_Pngpath[L"Kaho"], Filepath::m_Txtpath[L"Shader"]);
	g_Attack3->InitSet(pDevice, L"Basic", Filepath::m_Pngpath[L"Kaho"], Filepath::m_Txtpath[L"Shader"]);
	g_AirAttack->InitSet(pDevice, L"Basic", Filepath::m_Pngpath[L"Kaho"], Filepath::m_Txtpath[L"Shader"]);
#pragma endregion

	m_pCurrentScene = new LobbyScene;
	m_pCurrentScene->setDevice(pDevice, pContext);
	m_pCurrentScene->Init();

//	S_Object.AddPlayer(g_Player);
}
bool SceneMgr::Frame()
{
	if (S_Input.getKeyState(DIK_LBUTTON) == Input::KEYSTATE::KEY_PUSH)
	{
		VolumeSet(g_fBGMVolume, -0.1f);
	}
	if (S_Input.getKeyState(DIK_RBUTTON) == Input::KEYSTATE::KEY_PUSH)
	{
		VolumeSet(g_fBGMVolume, +0.1f);
	}
	if (m_pCurrentScene->Frame() == false)
	{
		m_pCurrentScene->Release();
		m_pCurrentScene = getScene(m_pCurrentScene->getSceneChange());
	}
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
Scene* SceneMgr::getScene(const bool& isNextScene)
{
	Scene* retScene = nullptr;
	if (isNextScene == false)
	{
		--m_iSceneIndex;
	}
	else
	{
		++m_iSceneIndex;
	}
	switch (m_iSceneIndex)
	{
	case 1:
	{
		retScene = new GameScene1;
		retScene->setDevice(m_pDevice, m_pContext);
		if (isNextScene == false)
		{
			retScene->inverseInit();
		}
		else
			retScene->Init();
	}break;
	case 2:
	{
		retScene = new GameScene2;
		retScene->setDevice(m_pDevice, m_pContext);
		if (isNextScene == false)
		{
			retScene->inverseInit();
		}
		else
			retScene->Init();
	}break;
	case 3:
	{
		retScene = new GameScene3;
		retScene->setDevice(m_pDevice, m_pContext);
		if (isNextScene == false)
		{
			retScene->inverseInit();
		}
		else
			retScene->Init();
	}break;
	case 4:
	{
		retScene = new GameScene4;
		retScene->setDevice(m_pDevice, m_pContext);
		if (isNextScene == false)
		{
			retScene->inverseInit();
		}
		else
			retScene->Init();
	}break;
	case 5:
		break;
	}
	return retScene;
}