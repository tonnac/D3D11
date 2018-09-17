#include "Input.h"

Input::Input()
{
	ZeroMemory(m_KeyState, sizeof(KEYSTATE) * 256);
	ZeroMemory(m_csBuffer, sizeof(TCHAR) * 256);
	ZeroMemory(m_BeforeMouseState, sizeof(KEYSTATE) * 3);
	ZeroMemory(m_CurrentMouseState, sizeof(KEYSTATE) * 3);
}


bool Input::Init()
{
	return true;
}
bool Input::Frame()
{
	GetCursorPos(&m_MousePos);
	ScreenToClient(g_hWnd, &m_MousePos);
	for (int i = 0; i < 256; ++i)
	{
		m_KeyState[i] = KeyCheck(i);
	}
	for (int iButton = 0; iButton < 3; ++iButton)
	{
		if (m_BeforeMouseState[iButton] == KEYSTATE::KEY_PUSH)
		{
			if (m_CurrentMouseState[iButton] == KEYSTATE::KEY_PUSH)
			{
				m_CurrentMouseState[iButton] = KEYSTATE::KEY_HOLD;
			}
		}
		if (m_BeforeMouseState[iButton] == KEYSTATE::KEY_UP)
		{
			if (m_CurrentMouseState[iButton] == KEYSTATE::KEY_UP)
			{
				m_CurrentMouseState[iButton] = KEYSTATE::KEY_FREE;
			}
		}
		m_BeforeMouseState[iButton] = m_CurrentMouseState[iButton];
	}
	_stprintf_s(m_csBuffer, L"X : %d   Y : %d", m_MousePos.x, m_MousePos.y);
	return true;
}
bool Input::Render()
{
	TextOut(g_hOffScreenDC,0,17,m_csBuffer,static_cast<int>(wcslen(m_csBuffer)));
	return true;
}
bool Input::Release()
{
	return true;
}

void Input::MSGEvent(MSG msg)
{
	switch (msg.message)
	{
	case WM_LBUTTONDOWN:
		m_CurrentMouseState[0] = KEYSTATE::KEY_PUSH;
		break;
	case WM_LBUTTONUP:
		m_CurrentMouseState[0] = KEYSTATE::KEY_UP;
		break;
	case WM_MBUTTONDOWN:
		m_CurrentMouseState[1] = KEYSTATE::KEY_PUSH;
		break;
	case WM_MBUTTONUP:
		m_CurrentMouseState[1] = KEYSTATE::KEY_UP;
		break;
	case WM_RBUTTONDOWN:
		m_CurrentMouseState[2] = KEYSTATE::KEY_PUSH;
		break;
	case WM_RBUTTONUP:
		m_CurrentMouseState[2] = KEYSTATE::KEY_UP;
	}
}
bool Input::isPressAnyKey()
{
	for (int i = 0; i < 256; ++i)
	{
		if (m_KeyState[i] == KEYSTATE::KEY_PUSH)
		{
			return true;
		}
	}
	return false;
}
KEYSTATE Input::GetKey(DWORD dwKey)
{
	return m_KeyState[dwKey];
}
KEYSTATE Input::GetMouse(DWORD dwKey)
{
	switch (dwKey)
	{
	case VK_LBUTTON:
		return m_CurrentMouseState[0];
		break;
	case VK_MBUTTON:
		return m_CurrentMouseState[1];
		break;
	case VK_RBUTTON:
		return m_CurrentMouseState[2];
	default:
		return KEYSTATE::KEY_FREE;
	}
}
POINT Input::GetMousePos()
{
	return m_MousePos;
}
void Input::setKey(DWORD dwKey)
{
	m_KeyState[dwKey] = KEYSTATE::KEY_PUSH;
}
KEYSTATE Input::KeyCheck(DWORD dwKey)
{
	SHORT sKey = GetAsyncKeyState(dwKey);

	if (sKey & 0x8000)
	{
		if (m_KeyState[dwKey] == KEYSTATE::KEY_FREE ||
			m_KeyState[dwKey] == KEYSTATE::KEY_UP)
		{
			m_KeyState[dwKey] = KEYSTATE::KEY_PUSH;
		}
		else
			m_KeyState[dwKey] = KEYSTATE::KEY_HOLD;
	}
	else
	{
		if (m_KeyState[dwKey] == KEYSTATE::KEY_PUSH ||
			m_KeyState[dwKey] == KEYSTATE::KEY_HOLD)
		{
			m_KeyState[dwKey] = KEYSTATE::KEY_UP;
		}
		else
			m_KeyState[dwKey] = KEYSTATE::KEY_FREE;
	}
	return m_KeyState[dwKey];
}
