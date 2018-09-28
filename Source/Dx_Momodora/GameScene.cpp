#include "GameScene.h"

GameScene::GameScene(const std::tstring& Scenename) : Scene(Scenename)
{}
void GameScene::InitArrow(PlayerEffectPtr arrow)
{
	arrow->InitSet(m_pDevice, L"Basic", Filepath::m_Pngpath[L"Kaho"], Filepath::m_Txtpath[L"Shader"]);
}

bool GameScene::Frame()
{
	return S_Object.Frame();
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

GameScene1::GameScene1() : GameScene(L"GameScene1")
{}
bool GameScene1::Frame()
{
	if (g_Player->getCenterPos().x >= g_rtClient.right - 5.0f)
	{
		m_bSceneChange = true;
		return false;
	}
	S_Object.Frame();
	return true;
}

GameScene2::GameScene2() : GameScene(L"GameScene2")
{}
bool GameScene2::Frame()
{
	if (g_Player->getCenterPos().x <= 5.0f)
	{
		return false;
	}
	if (g_Player->getCenterPos().x >= g_rtClient.right - 5.0f)
	{
		m_bSceneChange = true;
		return false;
	}
	S_Object.Frame();
	return true;
}

GameScene3::GameScene3() : GameScene(L"GameScene3")
{}
bool GameScene3::Frame()
{
	if (g_Player->getCenterPos().x <= 5.0f)
	{
		return false;
	}
	if (g_Player->getCenterPos().x >= g_rtClient.right - 5.0f)
	{
		m_bSceneChange = true;
		return false;
	}
	S_Object.Frame();
	return true;
}
bool GameScene3::Release()
{
	return true;
}
bool GameScene3::inverseInit()
{
	return true;
}

GameScene4::GameScene4()
{}
bool GameScene4::Frame()
{
	return true;
}
bool GameScene4::Release()
{
	return true;
}
bool GameScene4::inverseInit()
{
	return true;
}

GameScene5::GameScene5()
{}
bool GameScene5::Frame()
{
	return true;
}
bool GameScene5::Release()
{
	return true;
}
bool GameScene5::inverseInit()
{
	return true;
}