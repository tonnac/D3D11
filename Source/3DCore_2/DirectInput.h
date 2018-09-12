#pragma once
#include "Define.h"
#include "dinput.h"

#define KEYDOWN(x) ((m_KeyState[(x)] & 0x80) ? true : false)
#define KEYUP(x) ((m_KeyState[(x)] & 0x80) ? false : true)

class DirectInput : public Singleton<DirectInput>
{
	friend class Singleton<DirectInput>;
private:
	DirectInput();
public:
	bool					Init();
	bool					Frame();
	bool					Render();
	bool					Release();
public:
	KEYSTATE				getKeyState(const DWORD& dwKey);
	void					SetAcquire(const bool& isActive);
	void					CopytoBefore();
private:
	bool					DeviceUnacquire();
	bool					DeviceAcquire();
	void					InitSet();
private:
	LPDIRECTINPUT8			m_pDi;
	LPDIRECTINPUTDEVICE8	m_pKey;
	LPDIRECTINPUTDEVICE8	m_pMouse;

	BYTE					m_KeyState[MAXKEYNUM];
	BYTE					m_BeforeKeyState[MAXKEYNUM];
	DIMOUSESTATE			m_MouseState;
	DIMOUSESTATE			m_BeforeMouseState;
};

#define S_Input DirectInput::getInst()