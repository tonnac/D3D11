#include "DirectInput.h"

DirectInput::DirectInput()
{
	ZeroMemory(m_CurrentKeyState, sizeof(BYTE)* MAXKEYNUM);
	ZeroMemory(m_BeforeKeyState , sizeof(BYTE)* MAXKEYNUM);
	ZeroMemory(&m_CurrentMouseState, sizeof(DIMOUSESTATE));
	ZeroMemory(&m_BeforeMouseState, sizeof(DIMOUSESTATE));
}
bool DirectInput::Init()
{
	ThrowifFailed(DirectInput8Create(g_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&m_pDi, nullptr));

	ThrowifFailed(m_pDi->CreateDevice(GUID_SysKeyboard, &m_pKey, nullptr));
	ThrowifFailed(m_pKey->SetDataFormat(&c_dfDIKeyboard));
	ThrowifFailed(m_pKey->SetCooperativeLevel(g_hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND | DISCL_NOWINKEY));
	while (m_pKey->Acquire() == DIERR_INPUTLOST);

	ThrowifFailed(m_pDi->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr));
	ThrowifFailed(m_pMouse->SetDataFormat(&c_dfDIMouse));
	ThrowifFailed(m_pMouse->SetCooperativeLevel(g_hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND));
	while (m_pMouse->Acquire() == DIERR_INPUTLOST);
	return true;
}
bool DirectInput::Frame()
{
	if (FAILED(m_pKey->GetDeviceState(sizeof(BYTE) * MAXKEYNUM, m_CurrentKeyState)))
	{
		while (m_pKey->Acquire() == DIERR_INPUTLOST);
		m_pKey->Acquire();
	}
	if (FAILED(m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_CurrentMouseState)))
	{
		while (m_pMouse->Acquire() == DIERR_INPUTLOST);
		m_pMouse->Acquire();
	}
	return true;
}
bool DirectInput::PostFrame()
{
	memcpy(m_BeforeKeyState, m_CurrentKeyState, sizeof(BYTE) * MAXKEYNUM);
	memcpy(&m_BeforeMouseState, &m_CurrentMouseState, sizeof(DIMOUSESTATE));
	return true;
}
bool DirectInput::Render()
{
	return true;
}
bool DirectInput::Release()
{
	SetAcquire(false);
	RELEASE(m_pMouse);
	RELEASE(m_pKey);
	RELEASE(m_pDi);
	return true;
}
Input::MousePos DirectInput::getMousePos()
{
	return { m_CurrentMouseState.lX,m_CurrentMouseState.lY, m_CurrentMouseState.lZ };
}
Input::KEYSTATE	DirectInput::getKeyState(DWORD dwKey)
{
	if (dwKey & 0x100)
	{
		int ButtonNum = 0;
		switch (dwKey)
		{
		case DIK_LBUTTON:
			ButtonNum = 0;
			break;
		case DIK_RBUTTON:
			ButtonNum = 1;
			break;
		case DIK_MBUTTON:
			ButtonNum = 2;
		}
		if (KEYDOWN(m_BeforeMouseState.rgbButtons[ButtonNum]))
		{
			if (KEYDOWN(m_CurrentMouseState.rgbButtons[ButtonNum]))
			{
				return Input::KEYSTATE::KEY_HOLD;
			}
			else
			{
				return Input::KEYSTATE::KEY_UP;
			}
		}
		else
		{
			if (KEYDOWN(m_CurrentMouseState.rgbButtons[ButtonNum]))
			{
				return Input::KEYSTATE::KEY_PUSH;
			}
			else
			{
				return Input::KEYSTATE::KEY_FREE;
			}
		}
	}
	else
	{
		if (m_BeforeKeyState[dwKey] & 0x80)
		{
			if (KEYDOWN(m_CurrentKeyState[dwKey]))
			{
				return Input::KEYSTATE::KEY_HOLD;
			}
			else
			{
				return Input::KEYSTATE::KEY_UP;
			}
		}
		else
		{
			if (KEYDOWN(m_CurrentKeyState[dwKey]))
			{
				return Input::KEYSTATE::KEY_PUSH;
			}
			else
			{
				return Input::KEYSTATE::KEY_FREE;
			}
		}
	}
}
Input::KEYSTATE	DirectInput::getMouseState(DWORD dwKey)
{
	if (m_BeforeMouseState.rgbButtons[dwKey] & 0x80)
	{
		if (KEYDOWN(m_CurrentMouseState.rgbButtons[dwKey]))
		{
			return Input::KEYSTATE::KEY_HOLD;
		}
		else
		{
			return Input::KEYSTATE::KEY_UP;
		}
	}
	else
	{
		if (KEYDOWN(m_CurrentMouseState.rgbButtons[dwKey]))
		{
			return Input::KEYSTATE::KEY_PUSH;
		}
		else
		{
			return Input::KEYSTATE::KEY_FREE;
		}
	}
}
void DirectInput::SetAcquire(const bool& flag)
{
	if (m_pMouse == nullptr || m_pKey == nullptr) return;
	if (flag == true)
	{
		m_pKey->Acquire();
		m_pMouse->Acquire();
	}
	else
	{
		m_pKey->Unacquire();
		m_pMouse->Unacquire();
	}
}