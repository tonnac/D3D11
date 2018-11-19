#pragma once
#include "d3dUtil.h"

#define KEYDOWN(x) (x) & 0x80 ? true : false
#define KEYUP(x) (x) & 0x80 ? false : true

constexpr UINT16 MAXKEYNUM = 256;
constexpr UINT32 DIK_LBUTTON = 0x100;
constexpr UINT32 DIK_RBUTTON = 0x101;
constexpr UINT32 DIK_MBUTTON = 0x102;

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

public:
	DIMOUSESTATE			getMousePos()const;
	Input::KEYSTATE			getKeyState(DWORD dwKey)const;

private:
	void					SetAcquire(const bool& flag);

private:
	Microsoft::WRL::ComPtr<IDirectInput8>		m_pDi = nullptr;
	Microsoft::WRL::ComPtr<IDirectInputDevice8>	m_pKey = nullptr;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> m_pMouse = nullptr;

	BYTE					m_CurrentKeyState[MAXKEYNUM];
	BYTE					m_BeforeKeyState[MAXKEYNUM];

	DIMOUSESTATE			m_CurrentMouseState;
	DIMOUSESTATE			m_BeforeMouseState;
};


#define S_Input DirectInput::getInst()