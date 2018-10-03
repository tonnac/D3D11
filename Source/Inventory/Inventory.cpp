#include "Inventory.h"
#include "Player.h"

Inventory::Inventory() : m_iConsumable(0), m_iEquip(0)
{}

bool Inventory::InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
	const std::string& VSFunc, const std::string& PSFunc)
{
	UI::InitSet(pDevice, Name, TexFilepath, ShaderFilepath, VSFunc, PSFunc);
	D3DXVECTOR2 TexPos = m_Object.getTexture()->getImageSize();
	SetTexPos({ 0.0f,0.0f,TexPos.x,TexPos.y });
	SetCenterPos({ g_rtClient.right * 0.5f, g_rtClient.bottom * 0.5f });

	m_VertexList[0].Color = { 0.0f, 1.0f, 0.0f, 1.0f };
	m_VertexList[1].Color = { 1.0f, 0.0f, 0.0f, 1.0f };
	m_VertexList[2].Color = { 0.0f, 0.0f, 1.0f, 1.0f };
	m_VertexList[3].Color = { 0.5f, 0.5f, 0.5f, 1.0f };

	m_itemslotCen[0] = { 87.0f, 156.0f };
	m_itemslotCen[1] = { 165.0f, 156.0f };
	m_itemslotCen[2] = { 243.0f, 156.0f };
	m_itemslotCen[3] = { 87.0f, 321.0f };
	m_itemslotCen[4] = { 165.0f, 321.0f };

	m_Font.InitSet(pDevice, L"Basic", Filepath::m_Pngpath[L"None"], Filepath::m_Txtpath[L"Shader"]);
	m_Font.SetCenterPos({ 618.0f, 150.0f });

	m_SelectBar.InitSet(pDevice, L"SelectBar", Filepath::m_Txtpath[L"Shader"], "VS", "SelectPSS");
	m_SelectBar.SetCenterPos({ 685.5f, 150.0f });

	m_Button.InitSet(pDevice, L"Select", Filepath::m_Pngpath[L"InvenSel"], Filepath::m_Txtpath[L"Shader"], "VS", "SelectPS");
	m_Button.SetCenterPos(m_itemslotCen[m_iIndex]);

	return true;
}

bool Inventory::Frame()
{
	StateFrame();
	m_LengthDiff.x = (m_VertexList[1].TexPos.x - m_VertexList[0].TexPos.x) * 0.5f;
	m_LengthDiff.y = (m_VertexList[2].TexPos.y - m_VertexList[0].TexPos.y) * 0.5f;
	m_Font.Frame();
	m_Button.Frame();
	if (m_State == INVENSTATE::CONSUMASELECT)
	{
		m_SelectBar.Frame();
	}
	return UI::Frame();
}

bool Inventory::Render(ID3D11DeviceContext* pContext)
{
	Menu::Render(pContext);
	if (m_State == INVENSTATE::MAIN)
	{
		m_Button.Render(pContext);
	}
	if (m_State == INVENSTATE::CONSUMASELECT || m_State == INVENSTATE::EQUIPSELECT)
	{
		m_SelectBar.Render(pContext);
	}
	m_Font.Render(pContext);
	return true;
}
bool Inventory::Release()
{
	m_Button.Release();
	m_SelectBar.Release();
	return Menu::Release();
}
bool Inventory::IndexFrame()
{
	switch (m_iIndex)
	{
	case 0:
	{
		RightKey();
		if (LeftKey() == true)
		{
			m_iIndex = 4;
		}
		if (DownKey() == true)
		{
			m_iIndex = 3;
		}
		if (UPKey() == true)
		{
			m_iIndex = 3;
		}
	}break;
	case 1:
	{
		RightKey();
		LeftKey();
		if (DownKey() == true)
		{
			m_iIndex = 4;
		}
		if (UPKey() == true)
		{
			m_iIndex = 4;
		}
	}break;
	case 2:
	{
		RightKey();
		LeftKey();
		if (DownKey() == true)
		{
			m_iIndex = 4;
		}
		if (UPKey() == true)
		{
			m_iIndex = 4;
		}
	}break;
	case 3:
	{
		RightKey();
		LeftKey();
		if (DownKey() == true)
		{
			m_iIndex = 0;
		}
		if (UPKey() == true)
		{
			m_iIndex = 0;
		}
	}break;
	case 4:
	{
		if (RightKey() == true)
		{
			m_iIndex = 0;
		}
		LeftKey();
		if (DownKey() == true)
		{
			m_iIndex = 1;
		}
		if (UPKey() == true)
		{
			m_iIndex = 1;
		}
	}break;
	default:
		break;
	}
	if (m_iIndex < 3 && m_iIndex >= 0 && S_Input.getKeyState(DIK_A) == Input::KEYSTATE::KEY_PUSH)
	{
		m_State = INVENSTATE::CONSUMASELECT;
		return true;
	}
	else if (S_Input.getKeyState(DIK_A) == Input::KEYSTATE::KEY_PUSH)
	{
		m_State = INVENSTATE::EQUIPSELECT;
		return true;
	}
	m_Button.SetCenterPos(m_itemslotCen[m_iIndex]);
	return true;
}

bool Inventory::StateFrame()
{
	switch (m_State)
	{
	case INVENSTATE::MAIN:
	{
		IndexFrame();
	}break;
	case INVENSTATE::CONSUMASELECT:
	{
		if (S_Input.getKeyState(DIK_UP) == Input::KEYSTATE::KEY_PUSH)// && m_iConsumable < g_Player->getConsumableNum())
		{
			--m_iConsumable;
		}
		if (S_Input.getKeyState(DIK_DOWN) == Input::KEYSTATE::KEY_PUSH)// && m_iConsumable < g_Player->getConsumableNum())
		{
			++m_iConsumable;
		}
		if (S_Input.getKeyState(DIK_S) == Input::KEYSTATE::KEY_PUSH)
		{
			m_State = INVENSTATE::MAIN;
			m_iConsumable = 0;
			return true;
		}
		m_SelectBar.SetCenterPos({ 685.5f, 150.0f + (50.0f * m_iConsumable) });
	}break;
	case INVENSTATE::EQUIPSELECT:
	{
		if (S_Input.getKeyState(DIK_S) == Input::KEYSTATE::KEY_PUSH)
		{
			m_State = INVENSTATE::MAIN;
			m_iEquip = 0;
			return true;
		}
	}break;
	default:
		break;
	}
	return true;
}
