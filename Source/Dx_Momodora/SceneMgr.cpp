#include "SceneMgr.h"
#include "mSound.h"

SceneMgr::SceneMgr() : m_nIndex(0)
{
}

bool SceneMgr::Init()
{
	m_pCurrentScene = CreateScene(0, true);
	return true;
}
bool SceneMgr::Frame()
{
	if (S_Input.GetKey(VK_END) == KEYSTATE::KEY_PUSH)
	{
		g_pPlayer->setInvincible(!g_pPlayer->isInvincible());
	}
	if (!m_pCurrentScene->Frame())
	{
		bool bflag = m_pCurrentScene->getNextScene();
		m_pCurrentScene->Release();
		delete m_pCurrentScene;
		if (bflag)
		{
			m_pCurrentScene = CreateScene(++m_nIndex, bflag);
		}
		else
		{
			m_pCurrentScene = CreateScene(--m_nIndex, bflag);
		}
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
	m_pCurrentScene->Release();
	delete m_pCurrentScene;
	delete g_UI;
	delete g_HPBar;
	delete g_pPlayer;
	return true;
}
Scene* SceneMgr::CreateScene(const INT& index, const bool& bNextScene)
{
	Scene * pScene = nullptr;
	switch (index)
	{
	case 0:
		pScene = new LobbyScene;
		break;
	case 1:
		pScene = new GameScene1;
		break;
	case 2:
		pScene = new GameScene2;
		break;
	case 3:
		pScene = new GameScene3;
		break;
	case 4:
		pScene = new GameScene4;
		break;
	case 5:
		pScene = new GameScene5;
		break;
	case 6:
		pScene = new EndScene;
		break;
	}
	pScene->Init();
	if (!bNextScene)
	{
		pScene->inverseSet();
	}
	return pScene;
}