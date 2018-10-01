#include "InGameMenu.h"

InGameMenu::InGameMenu() : m_State(IGMSTATE::DEFAULT)
{}

bool InGameMenu::InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
						const std::string& VSFunc, const std::string& PSFunc)
{
	D3DXVECTOR2 TexPos;

	m_ConstantData.Color.x = 0.5f;

	m_Bar.SetCenterPos({ g_rtClient.right * 0.5f, 409.5f });
	m_Bar.InitSet(pDevice, L"Key", Filepath::m_Pngpath[L"Key"], Filepath::m_Txtpath[L"Shader"],"VS","KeyPS");
	m_Bar.SetTexPos({ 0.0f,0.0f,960.0f,51.0f });

	m_Button[0].SetCenterPos({ 552.0f,340.5f });
	m_Button[1].SetCenterPos({ 627.0f,343.5f });
	m_Button[2].SetCenterPos({ 697.5f,343.5f });
	m_Button[3].SetCenterPos({ 768.0f,342.0f });
	m_Button[4].SetCenterPos({ 846.0f,343.5f });

	m_Button[0].InitSet(pDevice, L"Button", Filepath::m_Pngpath[L"Item_IC"], Filepath::m_Txtpath[L"Shader"], "Button");
	m_Button[1].InitSet(pDevice, L"Button", Filepath::m_Pngpath[L"MI_IC"], Filepath::m_Txtpath[L"Shader"], "Button");
	m_Button[2].InitSet(pDevice, L"Button", Filepath::m_Pngpath[L"Map_IC"], Filepath::m_Txtpath[L"Shader"], "Button");
	m_Button[3].InitSet(pDevice, L"Button", Filepath::m_Pngpath[L"Setting_IC"], Filepath::m_Txtpath[L"Shader"], "Button");
	m_Button[4].InitSet(pDevice, L"Button", Filepath::m_Pngpath[L"Maple"], Filepath::m_Txtpath[L"Shader"], "Button");

	m_Button[0].LoadResourceView(pDevice, Filepath::m_Pngpath[L"Item_Acti"]);
	m_Button[1].LoadResourceView(pDevice, Filepath::m_Pngpath[L"MI_Acti"]);
	m_Button[2].LoadResourceView(pDevice, Filepath::m_Pngpath[L"Map_Acti"]);
	m_Button[3].LoadResourceView(pDevice, Filepath::m_Pngpath[L"Setting_Acti"]);
	m_Button[4].LoadResourceView(pDevice, Filepath::m_Pngpath[L"MainMenu_Acti"]);

	SetCenterPos({ g_rtClient.right * 0.5f, g_rtClient.bottom * 0.5f });

	m_Font[0].SetCenterPos({ 82.5f,345.0f });
	m_Font[1].SetCenterPos({ 142.5f,345.0f });
	m_Font[2].SetCenterPos({ 87.0f,345.0f });
	m_Font[3].SetCenterPos({ 82.5f,345.0f });
	m_Font[4].SetCenterPos({ 238.5f,345.0f });

	m_Font[0].InitSet(pDevice, L"Basic", Filepath::m_Pngpath[L"Item_F"], Filepath::m_Txtpath[L"Shader"]);
	m_Font[1].InitSet(pDevice, L"Basic", Filepath::m_Pngpath[L"MI_F"], Filepath::m_Txtpath[L"Shader"]);
	m_Font[2].InitSet(pDevice, L"Basic", Filepath::m_Pngpath[L"Map_F"], Filepath::m_Txtpath[L"Shader"]);
	m_Font[3].InitSet(pDevice, L"Basic", Filepath::m_Pngpath[L"Setting_F"], Filepath::m_Txtpath[L"Shader"]);
	m_Font[4].InitSet(pDevice, L"Basic", Filepath::m_Pngpath[L"MainMenu_F"], Filepath::m_Txtpath[L"Shader"]);

	Menu::InitSet(pDevice, Name, TexFilepath, ShaderFilepath, VSFunc, PSFunc);
	TexPos = m_Object.getTexture()->getImageSize();
	SetTexPos({0.0f,0.0f, TexPos.x, TexPos.y});
	return true;
}

bool InGameMenu::Frame()
{
	for (int i = 0; i < 5; ++i)
	{
		if (m_iIndex == i)
		{
			m_Button[i].setActive(true);
		}
		else
		{
			m_Button[i].setActive(false);
		}
		m_Button[i].Frame();
	}
	m_Bar.Frame();
	m_LengthDiff.x = (m_VertexList[1].TexPos.x - m_VertexList[0].TexPos.x) * 0.5f;
	m_LengthDiff.y = (m_VertexList[2].TexPos.y - m_VertexList[0].TexPos.y) * 0.5f;
	StateFrame();
	return Menu::Frame();
}
bool InGameMenu::Render(ID3D11DeviceContext* pContext)
{
	Menu::Render(pContext);
	for (auto & it : m_Button)
	{
		it.Render(pContext);
	}
	m_Bar.Render(pContext);
	m_Font[m_iIndex].Render(pContext);
	return true;
}
bool InGameMenu::Release()
{
	for (auto & it : m_Button)
	{
		it.Release();
	}
	for (int i = 0; i < 5; ++i)
	{
		m_Font[i].Release();
	}
	m_Bar.Release();
	return Menu::Release();
}

bool InGameMenu::RightKey()
{
	if (Menu::RightKey() == true)
	{
		for (int i = 0; i < 5; ++i)
		{
			m_Button[i].setMove(true);
		}
		return true;
	}
	return false;
}
bool InGameMenu::LeftKey()
{
	if (Menu::LeftKey() == true)
	{
		for (int i = 0; i < 5; ++i)
		{
			m_Button[i].setMove(true);
		}
		return true;
	}
	return false;
}

bool InGameMenu::StateFrame()
{
	if (LeftKey() == true && m_iIndex == -1)
	{
		m_iIndex = 4;
	}
	if (RightKey() == true && m_iIndex == 5)
	{
		m_iIndex = 0;
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
			m_State = IGMSTATE::SETTING;
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
	m_Font[m_iIndex].Frame();
	return true;
}