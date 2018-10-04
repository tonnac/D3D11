#include "SceneMgr.h"
#include "KahoAttack.h"
#include "Inventory.h"
#include "LobbyMenu.h"
#include "Bar.h"
#include "GameUI.h"

std::shared_ptr<PlayerEffect>	g_Attack1 = nullptr;
std::shared_ptr<PlayerEffect>	g_Attack2 = nullptr;
std::shared_ptr<PlayerEffect>	g_Attack3 = nullptr;
std::shared_ptr<PlayerEffect>	g_AirAttack = nullptr;
std::shared_ptr<Player>			g_Player = nullptr;
std::shared_ptr<Fade>			g_Fade = nullptr;
std::shared_ptr<Setting>		g_Setting = nullptr;
std::shared_ptr<InGameMenu>		g_IGM = nullptr;
std::shared_ptr<Inventory>		g_Inven = nullptr;
std::shared_ptr<GameUI>			g_GameUI = nullptr;

SceneMgr::SceneMgr() : m_iSceneIndex(3), m_iCount(0)
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

	TextureInit();

	g_Fade = std::make_shared<Fade>();
	g_Fade->InitSet(pDevice, L"Fade", Filepath::m_Txtpath[L"Shader"], "VS", "FadePS");

	g_Setting = std::make_shared<Setting>();
	g_Setting->InitSet(m_pDevice, L"Setting", Filepath::m_Pngpath[L"Lobby"], Filepath::m_Txtpath[L"Shader"], "VS", "SettingPS");

	g_IGM = std::make_shared<InGameMenu>();
	g_IGM->InitSet(m_pDevice, L"Terrain", Filepath::m_Pngpath[L"InGameMenu"], Filepath::m_Txtpath[L"Shader"], "VS", "TerrainPS");

	g_Inven = std::make_shared<Inventory>();
	g_Inven->InitSet(m_pDevice, L"Basic", Filepath::m_Pngpath[L"Inventory"], Filepath::m_Txtpath[L"Shader"]);

	g_GameUI = std::make_shared<GameUI>();
	g_GameUI->InitSet(m_pDevice);

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

	m_pCurrentScene = getScene(false);

	S_Object.AddPlayer(g_Player);
}
bool SceneMgr::Frame()
{
	if (isEndScene == false && g_Player->getHP() <= 0)
	{
		isEndScene = true;
		setEndScene();
	}
	if (m_pCurrentScene->Frame() == false)
	{
		m_pCurrentScene->Release();
		bool PrevScene = m_pCurrentScene->getPrevScene();
		delete m_pCurrentScene;
		m_pCurrentScene = getScene(PrevScene);
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
void SceneMgr::setGameMenu(const bool& flag)
{
	GameScene * pScene = dynamic_cast<GameScene*>(m_pCurrentScene);
	if (pScene != nullptr)
	{
		pScene->setGameMenu(flag);
	}
}
void SceneMgr::setLobbyScene()
{
	m_pCurrentScene->Release();
	delete m_pCurrentScene;
	m_iSceneIndex = -1;
	m_pCurrentScene = getScene(false);
}
void SceneMgr::setEndScene()
{
	m_pCurrentScene->Release();
	delete m_pCurrentScene;
	m_iSceneIndex = -1;
	m_pCurrentScene = new EndScene;
	m_pCurrentScene->setDevice(m_pDevice, m_pContext);
	m_pCurrentScene->Init();
}
void SceneMgr::TextureInit()
{
	S_Texture.LoadTexture(m_pDevice, L"0", Filepath::m_Pngpath[L"0"]);
	S_Texture.LoadTexture(m_pDevice, L"1", Filepath::m_Pngpath[L"1"]);
	S_Texture.LoadTexture(m_pDevice, L"2", Filepath::m_Pngpath[L"2"]);
	S_Texture.LoadTexture(m_pDevice, L"3", Filepath::m_Pngpath[L"3"]);
	S_Texture.LoadTexture(m_pDevice, L"4", Filepath::m_Pngpath[L"4"]);
	S_Texture.LoadTexture(m_pDevice, L"5", Filepath::m_Pngpath[L"5"]);
	S_Texture.LoadTexture(m_pDevice, L"6", Filepath::m_Pngpath[L"6"]);
	S_Texture.LoadTexture(m_pDevice, L"7", Filepath::m_Pngpath[L"7"]);
	S_Texture.LoadTexture(m_pDevice, L"8", Filepath::m_Pngpath[L"8"]);
	S_Texture.LoadTexture(m_pDevice, L"9", Filepath::m_Pngpath[L"9"]);

	S_Texture.LoadTexture(m_pDevice, L"Seed.png", Filepath::m_Pngpath[L"Seed"]);
	S_Texture.LoadTexture(m_pDevice, L"SeedDesc.png", Filepath::m_Pngpath[L"SeedDesc"]);
	S_Texture.LoadTexture(m_pDevice, L"SeedName.png", Filepath::m_Pngpath[L"SeedName"]);

	S_Texture.LoadTexture(m_pDevice, L"Flower.png", Filepath::m_Pngpath[L"Flower"]);
	S_Texture.LoadTexture(m_pDevice, L"FlowerDesc.png", Filepath::m_Pngpath[L"FlowerDesc"]);
	S_Texture.LoadTexture(m_pDevice, L"FlowerName.png", Filepath::m_Pngpath[L"FlowerName"]);
}
Scene* SceneMgr::getScene(const bool& isPrevScene)
{
	Scene* retScene = nullptr;
	if (isPrevScene == true)
	{
		--m_iSceneIndex;
	}
	else
	{
		++m_iSceneIndex;
	}
	switch (m_iSceneIndex)
	{
	case 0:
	{
		retScene = new LobbyScene;
	}break;
	case 1:
	{
		retScene = new GameScene1;
	}break;
	case 2:
	{
		retScene = new GameScene2;
	}break;
	case 3:
	{
		retScene = new GameScene3;
	}break;
	case 4:
	{
		retScene = new GameScene4;
	}break;
	case 5:
		retScene = new GameScene5;
		break;
	}
	if (m_iSceneIndex == 0)
	{
		std::shared_ptr<Lobbymenu> pLobby = std::make_shared<Lobbymenu>();
		pLobby->InitSet(m_pDevice, L"Basic", Filepath::m_Pngpath[L"Lobby"], Filepath::m_Txtpath[L"Shader"]);
		LobbyScene* pLobbyScene = dynamic_cast<LobbyScene*>(retScene);
		pLobbyScene->setLobby(pLobby);
	}
	retScene->setDevice(m_pDevice, m_pContext);
	if (isPrevScene == true)
	{
		retScene->inverseInit();
	}
	else
		retScene->Init();
	return retScene;
}