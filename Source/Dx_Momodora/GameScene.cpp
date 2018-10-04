#include "GameScene.h"
#include "InGameMenu.h"
#include "GameUI.h"

GameScene::GameScene(const std::tstring& Scenename) : Scene(Scenename), m_bGameMenu(false)
{}
void GameScene::InitArrow(PlayerEffectPtr arrow)
{
	arrow->InitSet(m_pDevice, L"Basic", Filepath::m_Pngpath[L"Kaho"], Filepath::m_Txtpath[L"Shader"]);
}
bool GameScene::getGameMenu() const
{
	return m_bGameMenu;
}
void GameScene::setGameMenu(const bool& flag)
{
	m_bGameMenu = flag;
}
bool GameScene::Frame()
{
	if (S_Input.getKeyState(DIK_LSHIFT) == Input::KEYSTATE::KEY_PUSH)
	{
		m_bGameMenu = true;
	}
	if (m_bGameMenu == true)
	{
		g_IGM->Frame();
	}
	else
	{
		if (m_bSceneChange == false && SceneChange() == true)
		{
			g_Fade->setDivideTime(0.5f);
			g_Fade->FadeOut();
		}
		if (g_Fade->getOn() == true)
		{
			g_Fade->Frame();
		}
		if (g_Fade->getOn() == false && m_bSceneChange == true)
		{
			return false;
		}
		g_GameUI->Frame();
		S_Object.Frame();
	}
	return true;
}
bool GameScene::Render()
{
	if (m_bGameMenu)
	{
		Scene::Render();
		return g_IGM->Render(m_pContext);
	}
	else
	{
		Scene::Render();
		g_GameUI->Render(m_pContext);
	}
	return true;
}
bool GameScene::Release()
{
	return S_Object.Release();
}
bool GameScene::inverseInit()
{
	SceneSet(true);
	return true;
}
bool GameScene::SceneChange()
{
	if (g_Player->getCenterPos().x <= 5.0f)
	{
		m_bPrevScene = true;
		m_bSceneChange = true;
		return true;
	}
	if (g_Player->getCenterPos().x >= g_rtClient.right - 5.0f)
	{
		m_bSceneChange = true;
		return true;
	}
	return false;
}

GameScene1::GameScene1() : GameScene(L"GameScene1")
{
	S_Sound.PlayBGM(BGM::STAGE);
}
bool GameScene1::SceneChange()
{
	if (g_Player->getCenterPos().x <= g_rtClient.left + 5.0f)
	{
		g_Player->MoveCenterPos({ 5.0f,0.0f });
	}
	if (g_Player->getCenterPos().x >= g_rtClient.right - 5.0f)
	{
		m_bSceneChange = true;
		return true;
	}
	return false;
}

GameScene2::GameScene2() : GameScene(L"GameScene2")
{}

GameScene3::GameScene3() : GameScene(L"GameScene3"), m_bAlram(false)
{}
void GameScene3::setDevice(ID3D11Device * pDevice, ID3D11DeviceContext* pContext)
{
	GameScene::setDevice(pDevice, pContext);
	m_Bar[0].InitSet(m_pDevice, L"Basic", Filepath::m_Pngpath[L"Inve"], Filepath::m_Txtpath[L"Shader"]);
	m_Bar[0].SetCenterPos({ 177.5f * 3.0f,145.0f * 3.0f });
	m_Bar[0].SetTexPos({ 0.0f,0.0f,228.0f,48.0f });
	m_Bar[1].InitSet(m_pDevice, L"Basic", Filepath::m_Pngpath[L"Acquire"], Filepath::m_Txtpath[L"Shader"]);
}
bool GameScene3::Frame()
{
	if (g_Player->getCenterPos().x > 141.0f * 3.0f && g_Player->getCenterPos().x < 191.0f * 3.0f)
	{
		m_bAlram = true;
	}
	else
	{
		m_bAlram = false;
	}
	if (m_bAlram == true)
	{
		m_Bar[0].Frame();
	}
	return GameScene::Frame();
}
bool GameScene3::Render()
{
	GameScene::Render();
	if (m_bAlram == true)
	{
		m_Bar[0].Render(m_pContext);
	}
	return true;
}
GameScene4::GameScene4() : GameScene(L"GameScene4")
{}
GameScene5::GameScene5()
{}
bool GameScene5::Frame()
{
	return true;
}