#include "GameUI.h"
#include "Player.h"

GameUI::~GameUI()
{
	Release();
}

bool GameUI::InitSet(ID3D11Device* pDevice)
{
	m_HPBar.SetCenterPos({ 205.0f, 61.5f });
	m_HPBar.SetTexPos({ 0.0f,0.0f,186.0f,21.0f });
	m_HPBar.InitSet(pDevice, L"Basic", Filepath::m_Pngpath[L"HPBar"], Filepath::m_Txtpath[L"Shader"]);

	m_HPBk.SetCenterPos({ 209.5f,61.5f });
	m_HPBk.SetTexPos({ 0.0f,0.0f,201.0f,27.0f });
	m_HPBk.InitSet(pDevice, L"Basic", Filepath::m_Pngpath[L"HPBK"], Filepath::m_Txtpath[L"Shader"]);

	m_Moo.SetCenterPos({ 157.5f, 97.5f });
	m_Moo.SetTexPos({ 0.0f,0.0f,101.0f,33.0f });
	m_Moo.InitSet(pDevice, L"Basic", Filepath::m_Pngpath[L"Moo"], Filepath::m_Txtpath[L"Shader"]);

	m_QuickBk.SetCenterPos({ 66.0f,84.0f });
	m_QuickBk.SetTexPos({ 0.0f,0.0f,72.0f,72.0f });
	m_QuickBk.InitSet(pDevice, L"Basic", Filepath::m_Pngpath[L"QuickSlot"], Filepath::m_Txtpath[L"Shader"]);

	m_QuickNum.SetCenterPos({ 66.0f,84.0f });
	m_QuickNum.InitSet(pDevice, L"Basic", Filepath::m_Txtpath[L"Shader"]);

	m_ItemNum.SetCenterPos({ 87.0f, 105.0f });
	m_ItemNum.InitSet(pDevice, L"Basic", Filepath::m_Txtpath[L"Shader"]);
	return true;
}

bool GameUI::Frame()
{
	m_HPBk.Frame();
	m_HPBar.Frame();
	m_Moo.Frame();
	m_QuickBk.Frame();
	QuickSlot Q = g_Inven->getQuickSlot();
	ItemPtr item = Q.getItem(g_Player->getQuickNum());
	if (item != nullptr)
	{
		INT Count = item->getCount();
		std::tstring num = std::to_tstring(Count);
		m_ItemNum.setTexture(S_Texture.LoadTexture(num));
		m_ItemNum.Frame();
	}
	m_QuickNum.setItem(item);
	m_QuickNum.Frame();
	return true;
}
bool GameUI::Render(ID3D11DeviceContext* pContext)
{
	m_HPBk.Render(pContext);
	m_HPBar.Render(pContext);
	m_Moo.Render(pContext);
	m_QuickBk.Render(pContext);
	m_QuickNum.Render(pContext);
	m_ItemNum.Render(pContext);
	return true;
}

bool GameUI::Release()
{
	m_HPBk.Release();
	m_HPBar.Release();
	m_Moo.Release();
	m_QuickBk.Release();
	m_QuickNum.Release();
	return true;
}