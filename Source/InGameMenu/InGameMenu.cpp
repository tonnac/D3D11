#include "InGameMenu.h"
#include "Setting.h"

InGameMenu::InGameMenu() : m_State(IGMSTATE::DEFAULT)
{}

bool InGameMenu::InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
						const std::string& VSFunc, const std::string& PSFunc)
{
	D3DXVECTOR2 TexPos;

	m_ConstantData.Color.x = 0.5f;

	m_Bar[0].SetCenterPos({ g_rtClient.right * 0.5f, 409.5f });
	m_Bar[0].InitSet(pDevice, L"Key", Filepath::m_Pngpath[L"Key"], Filepath::m_Txtpath[L"Shader"],"VS","KeyPS");
	m_Bar[0].SetTexPos({ 0.0f,0.0f,960.0f,51.0f });

	m_Bar[1].SetCenterPos({ 695.0f, 344.0f });
	m_Bar[1].InitSet(pDevice, L"Basic", Filepath::m_Pngpath[L"YoN"], Filepath::m_Txtpath[L"Shader"]);
	m_Bar[1].SetTexPos({ 0.0f,0.0f,468.0f,63.0f });

	m_Bar[2].SetCenterPos({ 519.5f, 343.5f });
	m_Bar[2].InitSet(pDevice, L"Basic", Filepath::m_Pngpath[L"Maple"], Filepath::m_Txtpath[L"Shader"]);
	m_Bar[2].SetTexPos({ 0.0f,0.0f,27.0f,27.0f });

	m_VertexList[0].Pos = { 0.0f, 0.0f, 0.5f };
	m_VertexList[1].Pos = { CASTING(FLOAT,g_rtClient.right), 0.0f, 0.5f };
	m_VertexList[2].Pos = { 0.0f, CASTING(FLOAT,g_rtClient.bottom), 0.5f };
	m_VertexList[3].Pos = { m_VertexList[1].Pos.x, m_VertexList[2].Pos.y, 0.5f };

	m_IngameBar.InitSet(pDevice, L"Basic", Filepath::m_Pngpath[L"IngameBar"], Filepath::m_Txtpath[L"Shader"]);

	m_ConstantData.Color.x = 0.0f;
	m_ConstantData.Color.y = 0.0f;
	m_ConstantData.Color.z = 0.0f;
	m_ConstantData.Color.w = 1.0f;

	CreateVertexBuffer(pDevice);
	m_Object.CreateShader(pDevice, Name, ShaderFilepath, VSFunc, PSFunc);
	m_Object.CreateRasterizer(pDevice);
	CreateIndexBuffer(pDevice);
	CreateConstantBuffer(pDevice);
	Init();
	return true;
}

bool InGameMenu::Frame()
{
	if (getSetting() == true)
	{
		g_Setting->Frame();
	}
	else if (m_State == IGMSTATE::DEFAULT)
	{
		m_IngameBar.Frame();
		m_Bar[0].Frame();
		return StateFrame();
	}
	else if (m_State == IGMSTATE::MAINMENU)
	{
		for (int i = 0; i < 3; ++i)
		{
			m_Bar[i].Frame();
		}
	}
	return true;
}
bool InGameMenu::Render(ID3D11DeviceContext* pContext)
{
	if (getSetting() == true)
	{
		g_Setting->Render(pContext);
	}
	else if (m_State == IGMSTATE::DEFAULT)
	{
		Menu::Render(pContext);
		m_IngameBar.Render(pContext);
		m_Bar[0].Render(pContext);
	}
	else if (m_State == IGMSTATE::MAINMENU)
	{
		for (int i = 0; i < 3; ++i)
		{
			m_IngameBar.Render(pContext);
			m_Bar[i].Render(pContext);
		}
	}
	return true;
}
bool InGameMenu::Release()
{
	m_IngameBar.Release();
	for (int i = 0; i < 3; ++i)
	{
		m_Bar[i].Release();
	}
	return Menu::Release();
}

bool InGameMenu::RightKey()
{
	if (Menu::RightKey() == true)
	{
		S_Sound.PlayEffect(Effect_Snd::MENUMOVE);
		m_IngameBar.setIndex(m_iIndex);
		m_IngameBar.setMove();
		return true;
	}
	return false;
}
bool InGameMenu::LeftKey()
{
	if (Menu::LeftKey() == true)
	{
		S_Sound.PlayEffect(Effect_Snd::MENUMOVE);
		m_IngameBar.setIndex(m_iIndex);
		m_IngameBar.setMove();
		return true;
	}
	return false;
}

bool InGameMenu::StateFrame()
{
	if (LeftKey() == true && m_iIndex == -1)
	{
		m_iIndex = 4;
		m_IngameBar.setIndex(m_iIndex);
	}
	if (RightKey() == true && m_iIndex == 5)
	{
		m_iIndex = 0;
		m_IngameBar.setIndex(m_iIndex);
	}

	switch (m_iIndex)
	{
	case 0:
		if (S_Input.getKeyState(DIK_A) == Input::KEYSTATE::KEY_PUSH)
		{
			m_State = IGMSTATE::INVENTORY;
		}
		break;
	case 1:
		if (S_Input.getKeyState(DIK_A) == Input::KEYSTATE::KEY_PUSH)
		{
			m_State = IGMSTATE::MAINITEM;
		}
		break;
	case 2:
		break;
	case 3:
		if (S_Input.getKeyState(DIK_A) == Input::KEYSTATE::KEY_PUSH)
		{
			setSetting(true);
			g_Setting->setMenu(this);
			S_Sound.PlayEffect(Effect_Snd::MENUSELECT);
		}
		break;
	case 4:
		if (S_Input.getKeyState(DIK_A) == Input::KEYSTATE::KEY_PUSH)
		{
			m_State = IGMSTATE::MAINMENU;
		}
		break;
	default:
		break;
	}
	return true;
}