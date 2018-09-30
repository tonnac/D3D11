#include "LobbyMenu.h"
#include "Bar.h"
#include "ObjectMgr.h"

Lobbymenu::Lobbymenu() : m_State(LOBBYSTATE::DEFAULT) 
{}

bool Lobbymenu::InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
	const std::string& VSFunc, const std::string& PSFunc)
{
	m_DrawArray[0] = { 0.0f,0.0f,960.0f,720.0f };
	m_DrawArray[1] = { 960.0f, 0.0f, 1920.0f, 720.0f };

	m_BarCenterPos[0] = { g_rtClient.right * 0.5f, 501.f };
	m_BarCenterPos[1] = { g_rtClient.right * 0.5f, 39.0f };
	m_BarCenterPos[2] = { g_rtClient.right * 0.5f, 123.0f };
	m_BarCenterPos[3] = { g_rtClient.right * 0.5f, 207.0f };
	m_BarCenterPos[4] = { g_rtClient.right * 0.5f, 291.0f };
	m_BarCenterPos[5] = { g_rtClient.right * 0.5f, 375.0f };
	m_BarCenterPos[6] = { 127.5f, 459.0f };
	m_BarCenterPos[7] = { 127.5f, 543.0f };

	m_BarDrawArray[0] = { 0.0f, 119.0f, 384.0f, 155.0f };
	m_BarDrawArray[1] = { 3.0f, 37.0f, 957.0f, 115.0f };
	m_BarDrawArray[2] = { 3.0f, 37.0f, 957.0f, 115.0f };
	m_BarDrawArray[3] = { 3.0f, 37.0f, 957.0f, 115.0f };
	m_BarDrawArray[4] = { 3.0f, 37.0f, 957.0f, 115.0f };
	m_BarDrawArray[5] = { 3.0f, 37.0f, 957.0f, 115.0f };
	m_BarDrawArray[6] = { 3.0f, 245.0f, 252.0f, 323.0f };
	m_BarDrawArray[7] = { 3.0f, 160.0f, 252.0f, 238.0f };

	m_StartCenterPos[0] = { 0.0f,0.0f};
	m_StartCenterPos[1] = { 808.5f, 44.0f};
	m_StartCenterPos[2] = { 808.5f, 128.0f};
	m_StartCenterPos[3] = { 808.5f, 212.0f};
	m_StartCenterPos[4] = { 808.5f, 296.0f};
	m_StartCenterPos[5] = { 808.5f, 380.0f};


	m_Bar = std::make_shared<Bar>();
	m_Bar->SetCenterPos(m_BarCenterPos[0]);
	m_Bar->SetTexPos(m_BarDrawArray[0]);
	m_Bar->InitSet(pDevice, Name, Filepath::m_Pngpath[L"Misc"], ShaderFilepath);

	m_Start = std::make_shared<Bar>();
	m_Start->InitSet(pDevice, Name, Filepath::m_Pngpath[L"Misc"], ShaderFilepath);
	m_Start->SetTexPos({ 491.0f, 121.0f, 598.0f, 161.0f });

	SetTexPos(m_DrawArray[0]);
	SetCenterPos({ g_rtClient.right * 0.5f, g_rtClient.bottom * 0.5f });
	g_Fade->FadeIn();
	g_Fade->setDivideTime(10.0f);
	return Menu::InitSet(pDevice, Name, TexFilepath, ShaderFilepath);
}

void Lobbymenu::SetPos(const FLOAT& rtLeft, const FLOAT& rtTop, const FLOAT& rtRight, const FLOAT& rtBottom)
{
	m_VertexList[0].TexPos = { rtLeft, rtTop };
	m_VertexList[1].TexPos = { rtRight, rtTop };
	m_VertexList[2].TexPos = { rtLeft, rtBottom };
	m_VertexList[3].TexPos = { rtRight, rtBottom };
	m_VertexList[0].Pos = { 0.0f, 0.0f, 0.5f };
	m_VertexList[1].Pos = { CASTING(FLOAT,g_rtClient.right), 0.0f, 0.5f };
	m_VertexList[2].Pos = { 0.0f, CASTING(FLOAT,g_rtClient.bottom), 0.5f };
	m_VertexList[3].Pos = { m_VertexList[1].Pos.x, m_VertexList[2].Pos.y, 0.5f };

}
bool Lobbymenu::Frame()
{
	if (getSetting() == true)
	{
		g_Setting->Frame();
	}
	else
	{
		m_LengthDiff.x = (m_VertexList[1].TexPos.x - m_VertexList[0].TexPos.x) * 0.5f;
		m_LengthDiff.y = (m_VertexList[2].TexPos.y - m_VertexList[0].TexPos.y) * 0.5f;
		if (StateFrame() == false)
		{
			return false;
		}
		Menu::Frame();
	}
	return true;
}

bool Lobbymenu::Render(ID3D11DeviceContext* pContext)
{
	if (getSetting() == true)
	{
		g_Setting->Render(pContext);
	}
	else
	{
		Menu::Render(pContext);
		if (m_State != LOBBYSTATE::DEFAULT)
		{
			m_Bar->Render(pContext);
		}
		if (m_State == LOBBYSTATE::SELECT)
		{
			m_Start->Render(pContext);
		}
	}
	return true;
}
bool Lobbymenu::Release()
{
	S_Object.AddPlayer(g_Player);
	return true;
}
bool Lobbymenu::DownKey()
{
	if (Menu::DownKey() == true)
	{
		S_Sound.PlayEffect(Effect_Snd::MENUMOVE);
	}
	return true;
}
bool Lobbymenu::UPKey()
{
	if (Menu::UPKey() == true)
	{
		S_Sound.PlayEffect(Effect_Snd::MENUMOVE);
	}
	return true;
}
void Lobbymenu::setState(const LOBBYSTATE& type)
{
	m_State = type;
}
bool Lobbymenu::StateFrame()
{
	if (m_State == LOBBYSTATE::DEFAULT)
	{
		if (g_Fade->getOn() == false)
		{
			m_State = LOBBYSTATE::START;
		}
	}
	switch (m_State)
	{
	case LOBBYSTATE::START:
		if (S_Input.PressAnyKey() == true)
		{
			S_Sound.PlayEffect(Effect_Snd::PRESSANYKEY);
			S_Sound.PlayBGM(BGM::TITLE);
//			S_Sound.PlayEffect(BGM::TITLE, true, true);
			m_State = LOBBYSTATE::MAINMENU;
			++m_iIndex;
		}
		break;
	case LOBBYSTATE::MAINMENU:
	{
		switch (m_iIndex)
		{
		case 1:
			if (S_Input.getKeyState(DIK_A) == Input::KEYSTATE::KEY_PUSH)
			{
				m_State = LOBBYSTATE::SELECT;
				m_Start->SetCenterPos(m_StartCenterPos[m_iIndex]);
				m_Start->Frame();
				S_Sound.PlayEffect(Effect_Snd::MENUSELECT);
			}
			DownKey();
			break;
		case 2:
		case 3:
		case 4:
		case 5:
			if (S_Input.getKeyState(DIK_A) == Input::KEYSTATE::KEY_PUSH)
			{
				m_State = LOBBYSTATE::SELECT;
				m_Start->SetCenterPos(m_StartCenterPos[m_iIndex]);
				m_Start->Frame();
				S_Sound.PlayEffect(Effect_Snd::MENUSELECT);
			}
			DownKey();
			UPKey();
			break;
		case 6:
			DownKey();
			UPKey();
			if (S_Input.getKeyState(DIK_A) == Input::KEYSTATE::KEY_PUSH)
			{
				setSetting(true);
				g_Setting->setMenu(this);
			}
			break;
		case 7:
			UPKey();
			if (S_Input.getKeyState(DIK_A) == Input::KEYSTATE::KEY_PUSH)
			{
				PostQuitMessage(0);
				return true;
			}
		}
		m_Bar->SetTexPos(m_BarDrawArray[m_iIndex]);
		m_Bar->SetCenterPos(m_BarCenterPos[m_iIndex]);
		SetTexPos(m_DrawArray[1]);
	}break;
	case LOBBYSTATE::SELECT:
		if (S_Input.getKeyState(DIK_S) == Input::KEYSTATE::KEY_PUSH)
		{
			S_Sound.PlayEffect(Effect_Snd::MENUCANCLE);
			m_State = LOBBYSTATE::MAINMENU;
		}
		if (S_Input.getKeyState(DIK_A) == Input::KEYSTATE::KEY_PUSH)
		{
			S_Sound.StopBGM(BGM::TITLE);
			S_Sound.PlayEffect(Effect_Snd::MENUSELECT);
			return false;
		}
		break;
	default:
		break;
	}
	if (m_State != LOBBYSTATE::DEFAULT)
	{
		m_Bar->Frame();
	}
	return true;
}