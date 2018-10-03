#include "InGameMenu.h"
#include "Setting.h"
#include "SceneMgr.h"
#include "Inventory.h"

InGameMenu::InGameMenu() : m_State(IGMSTATE::DEFAULT), m_iMapleIndex(0)
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

	m_Bar[3].SetCenterPos({ g_rtClient.right * 0.5f, g_rtClient.bottom * 0.5f });
	m_Bar[3].SetTexPos({ 0, 0, 960.0f, 720.0f });
	m_Bar[3].InitSet(pDevice, L"Basic", Filepath::m_Pngpath[L"MI"], Filepath::m_Txtpath[L"Shader"]);

	m_MaplePos[0] = { 519.5f, 343.5f };
	m_MaplePos[1] = { 711.5f, 343.5 };

	m_VertexList[0].Pos = { 0.0f, 0.0f, 0.5f };
	m_VertexList[1].Pos = { CASTING(FLOAT,g_rtClient.right), 0.0f, 0.5f };
	m_VertexList[2].Pos = { 0.0f, CASTING(FLOAT,g_rtClient.bottom), 0.5f };
	m_VertexList[3].Pos = { m_VertexList[1].Pos.x, m_VertexList[2].Pos.y, 0.5f };

	m_IngameBar.InitSet(pDevice, L"Basic", Filepath::m_Pngpath[L"IngameBar"], Filepath::m_Txtpath[L"Shader"]);

	m_ConstantData.Color.x = 0.0f;
	m_ConstantData.Color.y = 0.0f;
	m_ConstantData.Color.z = 0.0f;
	m_ConstantData.Color.w = 0.5f;

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
	return StateFrame();
}
bool InGameMenu::Render(ID3D11DeviceContext* pContext)
{
	StateRender(pContext);
	return true;
}
bool InGameMenu::Release()
{
	m_IngameBar.Release();
	for (int i = 0; i < 4; ++i)
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

bool InGameMenu::IndexFrame()
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
			S_Sound.PlayEffect(Effect_Snd::MENUSELECT);
			m_State = IGMSTATE::INVENTORY;
		}
		break;
	case 1:
		if (S_Input.getKeyState(DIK_A) == Input::KEYSTATE::KEY_PUSH)
		{
			S_Sound.PlayEffect(Effect_Snd::MENUSELECT);
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
			m_State = IGMSTATE::SETTING;
			S_Sound.PlayEffect(Effect_Snd::MENUSELECT);
		}
		break;
	case 4:
		if (S_Input.getKeyState(DIK_A) == Input::KEYSTATE::KEY_PUSH)
		{
			S_Sound.PlayEffect(Effect_Snd::MENUSELECT);
			m_State = IGMSTATE::MAINMENU;
		}
		break;
	default:
		break;
	}
	return true;
}
bool InGameMenu::StateFrame()
{
	switch (m_State)
	{
	case IGMSTATE::DEFAULT:
	{
		if (S_Input.getKeyState(DIK_S) == Input::KEYSTATE::KEY_PUSH)
		{
			S_Sound.PlayEffect(Effect_Snd::MENUCANCLE);
			S_Scene.setGameMenu(false);
			m_iIndex = 0;
			m_IngameBar.setIndex(m_iIndex);
		}
		IndexFrame();
		m_IngameBar.Frame();
		m_Bar[0].Frame();
	}break;
	case IGMSTATE::INVENTORY:
	{
		if (g_Inven->Frame() == false)
		{
			m_State = IGMSTATE::DEFAULT;
		}
	}break;
	case IGMSTATE::MAINITEM:
	{
		m_Bar[3].Frame();
		if (S_Input.getKeyState(DIK_S) == Input::KEYSTATE::KEY_PUSH)
		{
			S_Sound.PlayEffect(Effect_Snd::MENUCANCLE);
			m_State = IGMSTATE::DEFAULT;
		}
	}break;
	case IGMSTATE::SETTING:
	{
		g_Setting->Frame();
	}break;
	case IGMSTATE::MAINMENU:
	{
		if (S_Input.getKeyState(DIK_RIGHT) == Input::KEYSTATE::KEY_PUSH || 
			S_Input.getKeyState(DIK_LEFT) == Input::KEYSTATE::KEY_PUSH)
		{
			if (m_iMapleIndex == 0)
			{
				m_iMapleIndex = 1;
			}
			else
				m_iMapleIndex = 0;
		}
		if (S_Input.getKeyState(DIK_S) == Input::KEYSTATE::KEY_PUSH || 
			m_iMapleIndex == 1 && S_Input.getKeyState(DIK_A) == Input::KEYSTATE::KEY_PUSH)
		{
			S_Sound.PlayEffect(Effect_Snd::MENUCANCLE);
			m_iMapleIndex = 0;
			m_State = IGMSTATE::DEFAULT;
			break;
		}
		if (m_iMapleIndex == 0 && S_Input.getKeyState(DIK_A) == Input::KEYSTATE::KEY_PUSH)
		{
			S_Sound.StopBGM(BGM::STAGE);
			S_Scene.setLobbyScene();
			m_iMapleIndex = 0;
			m_State = IGMSTATE::DEFAULT;
			m_iIndex = 0;
			m_IngameBar.setIndex(m_iIndex);
			return true;
		}
		m_Bar[2].SetCenterPos(m_MaplePos[m_iMapleIndex]);
		for (int i = 0; i < 3; ++i)
		{
			m_Bar[i].Frame();
		}
	}break;
	default:
		break;
	}
	return true;
}
bool InGameMenu::StateRender(ID3D11DeviceContext* pContext)
{
	switch (m_State)
	{
	case IGMSTATE::DEFAULT:
	{
		Menu::Render(pContext);
		m_IngameBar.Render(pContext);
		m_Bar[0].Render(pContext);
	}break;
	case IGMSTATE::INVENTORY:
	{
		g_Inven->Render(pContext);
	}break;
	case IGMSTATE::MAINITEM:
	{
		m_Bar[3].Render(pContext);
	}break;
	case IGMSTATE::SETTING:
	{
		g_Setting->Render(pContext);
	}break;
	case IGMSTATE::MAINMENU:
	{
		Menu::Render(pContext);
		m_IngameBar.Render(pContext);
		for (int i = 0; i < 3; ++i)
		{
			m_Bar[i].Render(pContext);
		}
	}break;
	default:
		break;
	}
	return true;
}
IGMSTATE InGameMenu::getState() const
{
	return m_State;
}
void InGameMenu::setSetting(const bool& set)
{
	Menu::setSetting(set);
	if (set == true)
	{
		m_State = IGMSTATE::SETTING;
	}
	else
	{
		m_State = IGMSTATE::DEFAULT;
	}
}