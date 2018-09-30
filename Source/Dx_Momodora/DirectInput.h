#pragma once
#include "d3dUtil.h"

#define KEYDOWN(x) (x) & 0x80 ? true : false
#define KEYUP(x) (x) & 0x80 ? false : true

#define DIK_LBUTTON 0x100
#define DIK_RBUTTON 0x101
#define DIK_MBUTTON 0x102

namespace Input
{
	struct MousePos
	{
		LONG lX, lY, lZ;
	};
	enum class KEYSTATE : unsigned char
	{
		KEY_FREE,
		KEY_PUSH,
		KEY_HOLD,
		KEY_UP
	};
}

class DirectInput : public Singleton<DirectInput>
{
	friend class Singleton<DirectInput>;
private:
	DirectInput();
public:
	bool					Init();
	bool					Frame();
	bool					PostFrame();
	bool					Render();
	bool					Release();
public:
	POINT					getMousePos() const;
	Input::KEYSTATE			getKeyState(DWORD dwKey) const;
	bool					PressAnyKey();
public:
	void					SetAcquire(const bool& flag);
private:
	LPDIRECTINPUT8			m_pDi = nullptr;
	LPDIRECTINPUTDEVICE8	m_pKey = nullptr;
	LPDIRECTINPUTDEVICE8	m_pMouse = nullptr;

	BYTE					m_CurrentKeyState[MAXKEYNUM];
	BYTE					m_BeforeKeyState[MAXKEYNUM];

	DIMOUSESTATE			m_CurrentMouseState;
	DIMOUSESTATE			m_BeforeMouseState;

	POINT					m_MousePos;
};


#define S_Input DirectInput::getInst()