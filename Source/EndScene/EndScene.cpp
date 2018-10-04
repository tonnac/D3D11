#include "EndScene.h"

bool EndScene::Init()
{
	m_Empty.InitSet(m_pDevice, L"Terrain", Filepath::m_Txtpath[L"Shader"], "VS", "TerrainPS");
	m_Empty.SetPos({ 0.0f,0.0f,CASTING(FLOAT,g_rtClient.right), CASTING(FLOAT,g_rtClient.bottom) });
	m_Empty.setColor({ 1.0f,1.0f,0.0f,1.0f });
	m_fTimer = 0.0f;
	m_ftTimer = 0.0f;
	bFlag = false;
	m_EndScene.InitSet(m_pDevice, L"Basic", Filepath::m_Pngpath[L"EndScene"], Filepath::m_Txtpath[L"Shader"]);
	m_EndScene.SetTexPos({ 0.0f,0.0f, CASTING(FLOAT, g_rtClient.right), CASTING(FLOAT, g_rtClient.bottom) });
	m_EndScene.SetCenterPos({ g_rtClient.right * 0.5f, g_rtClient.bottom * 0.5f });
	return true;
}
bool EndScene::Frame()
{
	m_fTimer += g_fSecPerFrame;
	m_ftTimer += g_fSecPerFrame;

	if (m_ftTimer >= 1.2f)
	{
		m_Empty.setColor({ 0.0f,0.0f,0.0f,1.0f });
	}
	else if (bFlag == false && m_fTimer >= 0.05f)
	{
		m_fTimer -= 0.05f;
		bFlag = !bFlag;
		m_Empty.setColor({ 1.0f,0.0f,0.0f,1.0f });
	}
	else if (bFlag == true && m_fTimer >= 0.05)
	{
		m_fTimer -= 0.05f;
		bFlag = !bFlag;
		m_Empty.setColor({ 0.0f,0.0f,0.0f,1.0f });
	}
	if (m_ftTimer >= 3.0f)
	{
		m_EndScene.Frame();
		if (S_Input.getKeyState(DIK_A) == Input::KEYSTATE::KEY_PUSH)
		{
			DestroyWindow(g_hWnd);
			return true;
		}
	}
	else
	{
		m_Empty.Frame();
	}
	return true;
}
bool EndScene::Render()
{
	if (m_ftTimer >= 3.0f)
	{
		m_EndScene.Render(m_pContext);
	}
	else
	{
		m_Empty.Render(m_pContext);
	}
	return true;
}
bool EndScene::Release()
{
	m_Empty.Release();
	return true;
}