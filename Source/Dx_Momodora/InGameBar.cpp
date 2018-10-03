#include "InGameBar.h"
#include "InGameMenu.h"

bool InGameBar::InitSet(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& TexFilepath, const std::tstring& ShaderFilepath,
				const std::string& VSFunc, const std::string& PSFunc)
{
	m_iIndex = 0;

	m_Button[0].SetCenterPos({ 552.0f,340.5f });
	m_Button[1].SetCenterPos({ 627.0f,343.5f });
	m_Button[2].SetCenterPos({ 697.5f,343.5f });
	m_Button[3].SetCenterPos({ 768.0f,342.0f });
	m_Button[4].SetCenterPos({ 846.0f,343.5f });

	m_Button[0].InitSet(pDevice, L"Button", Filepath::m_Pngpath[L"Item_IC"], Filepath::m_Txtpath[L"Shader"], "Button");
	m_Button[1].InitSet(pDevice, L"Button", Filepath::m_Pngpath[L"MI_IC"], Filepath::m_Txtpath[L"Shader"], "Button");
	m_Button[2].InitSet(pDevice, L"Button", Filepath::m_Pngpath[L"Map_IC"], Filepath::m_Txtpath[L"Shader"], "Button");
	m_Button[3].InitSet(pDevice, L"Button", Filepath::m_Pngpath[L"Setting_IC"], Filepath::m_Txtpath[L"Shader"], "Button");
	m_Button[4].InitSet(pDevice, L"Button", Filepath::m_Pngpath[L"MainMenu_IC"], Filepath::m_Txtpath[L"Shader"], "Button");

	m_Button[0].LoadResourceView(pDevice, Filepath::m_Pngpath[L"Item_Acti"]);
	m_Button[1].LoadResourceView(pDevice, Filepath::m_Pngpath[L"MI_Acti"]);
	m_Button[2].LoadResourceView(pDevice, Filepath::m_Pngpath[L"Map_Acti"]);
	m_Button[3].LoadResourceView(pDevice, Filepath::m_Pngpath[L"Setting_Acti"]);
	m_Button[4].LoadResourceView(pDevice, Filepath::m_Pngpath[L"MainMenu_Acti"]);

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

	SetTexPos({ 0.0f,0.0f,960.0f,75.0f });
	SetCenterPos({ g_rtClient.right * 0.5f, 343.5f });

	return Bar::InitSet(pDevice, Name, TexFilepath, ShaderFilepath, VSFunc, PSFunc);
}
bool InGameBar::Frame()
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
	m_Font[m_iIndex].Frame();
	return Bar::Frame();
}
bool InGameBar::Render(ID3D11DeviceContext* pContext)
{
	Bar::Render(pContext);
	if (g_IGM->getState() == IGMSTATE::DEFAULT)
	{
		for (auto & it : m_Button)
		{
			it.Render(pContext);
		}
	}
	m_Font[m_iIndex].Render(pContext);
	return true;
}
bool InGameBar::Release()
{
	for (auto & it : m_Button)
	{
		it.Release();
	}
	for (int i = 0; i < 5; ++i)
	{
		m_Font[i].Release();
	}
	return Bar::Release();
}
void InGameBar::setMove(const bool& flag)
{
	for (int i = 0; i < 5; ++i)
	{
		m_Button[i].setMove(flag);
	}
}
INT	InGameBar::getIndex() const
{
	return m_iIndex;
}
void InGameBar::setIndex(const INT& index)
{
	m_iIndex = index;
}