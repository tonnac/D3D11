#include "Menu.h"
#include "mSound.h"

Menu::Menu() : m_iIndex(0)
{}

Menu::~Menu()
{
	Release();
}

void Menu::setSetting(const bool& set)
{
	m_bSetting = set;
}
bool Menu::getSetting() const
{
	return m_bSetting;
}

bool Menu::DownKey()
{
	if (S_Input.getKeyState(DIK_DOWN) == Input::KEYSTATE::KEY_PUSH)
	{
		++m_iIndex;
		return true;
	}
	return false;
}
bool Menu::UPKey()
{
	if (S_Input.getKeyState(DIK_UP) == Input::KEYSTATE::KEY_PUSH)
	{
		--m_iIndex;
		return true;
	}
	return false;
}

bool Menu::RightKey()
{
	if (S_Input.getKeyState(DIK_RIGHT) == Input::KEYSTATE::KEY_PUSH)
	{
		++m_iIndex;
		return true;
	}
	return false;
}
bool Menu::LeftKey()
{
	if (S_Input.getKeyState(DIK_LEFT) == Input::KEYSTATE::KEY_PUSH)
	{
		--m_iIndex;
		return true;
	}
	return false;
}