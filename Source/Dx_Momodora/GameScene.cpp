#include "GameScene.h"

GameScene::GameScene(const std::tstring& Scenename) : Scene(Scenename)
{}
void GameScene::InitArrow(PlayerEffectPtr arrow)
{
	arrow->InitSet(m_pDevice, L"Basic", Filepath::m_Pngpath[L"Kaho"], Filepath::m_Txtpath[L"Shader"]);
}

bool GameScene::Frame()
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
	S_Object.Frame();
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

GameScene3::GameScene3() : GameScene(L"GameScene3")
{}
bool GameScene3::Frame()
{
	return GameScene::Frame();
}

GameScene4::GameScene4()
{}
bool GameScene4::Frame()
{
	return true;
}
GameScene5::GameScene5()
{}
bool GameScene5::Frame()
{
	return true;
}