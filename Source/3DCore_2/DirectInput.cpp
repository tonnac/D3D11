#include "DirectInput.h"

DirectInput::DirectInput() : m_pDi(nullptr), m_pKey(nullptr), m_pMouse(nullptr)
{
	ZeroMemory(&m_pKey, sizeof(BYTE) * 256);
	ZeroMemory(&m_BeforeKeyState, sizeof(BYTE) * 256);
	ZeroMemory(&m_MouseState, sizeof(DIMOUSESTATE));
	ZeroMemory(&m_BeforeMouseState, sizeof(DIMOUSESTATE));

}

bool DirectInput::Init()
{
	if (FAILED(InitSet()))
	{
		return false;
	}
	return true;
}
bool DirectInput::Frame()
{
	HRESULT hr;
	hr = m_pKey->GetDeviceState(MAXKEYNUM, &m_KeyState);
	if (FAILED(hr))
	{
		while (m_pKey->Acquire() == DIERR_INPUTLOST);
	}
	hr = m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_MouseState);
	if (FAILED(hr))
	{
		while (m_pMouse->Acquire() == DIERR_INPUTLOST);
	}
	return true;
}
bool DirectInput::Render()
{
	return true;
}
bool DirectInput::Release()
{
	DeviceUnacquire();

	RELEASE(m_pKey);
	RELEASE(m_pMouse);
	RELEASE(m_pDi);
	return true;
}
KEYSTATE DirectInput::getKeyState(const DWORD& dwKey)
{
	if (m_BeforeKeyState[dwKey] & 0x80)
	{
		if (KEYDOWN(dwKey))
		{
			return KEYSTATE::KEY_HOLD;
		}
		else
		{
			return KEYSTATE::KEY_UP;
		}
	}
	else
	{
		if (KEYDOWN(dwKey))
		{
			return KEYSTATE::KEY_PUSH;
		}
		else
		{
			return KEYSTATE::KEY_FREE;
		}
	}
}
void DirectInput::SetAcquire(const bool& isActive)
{
	if (isActive == true)
	{
		DeviceAcquire();
	}
	else
	{
		DeviceUnacquire();
	}
}
void DirectInput::CopytoBefore()
{
	memcpy(m_BeforeKeyState, m_KeyState, sizeof(BYTE) * MAXKEYNUM);
	memcpy(&m_BeforeMouseState, &m_MouseState, sizeof(DIMOUSESTATE));
}
bool DirectInput::DeviceUnacquire()
{
	if (m_pKey) m_pKey->Unacquire();
	if (m_pMouse) m_pMouse->Unacquire();
	return true;
}
bool DirectInput::DeviceAcquire()
{
	if (m_pKey) m_pKey->Acquire();
	if (m_pMouse) m_pMouse->Acquire();
	return true;
}
HRESULT	DirectInput::InitSet()
{
	HRESULT hr;
	ThrowifFailed(DirectInput8Create(g_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, VOIDPTR(m_pDi), nullptr));

	ThrowifFailed(m_pDi->CreateDevice(GUID_SysKeyboard, &m_pKey, nullptr));
	ThrowifFailed(m_pKey->SetDataFormat(&c_dfDIKeyboard));
	ThrowifFailed(m_pKey->SetCooperativeLevel(g_hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND | DISCL_NOWINKEY));
	while (m_pKey->Acquire() == DIERR_INPUTLOST);

	ThrowifFailed(m_pDi->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr));
	ThrowifFailed(m_pMouse->SetDataFormat(&c_dfDIMouse));
	ThrowifFailed(m_pMouse->SetCooperativeLevel(g_hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND));
	while (m_pMouse->Acquire() == DIERR_INPUTLOST);

	return hr;
}