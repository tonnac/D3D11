#pragma once
#include "Std.h"

class Input : public Singleton<Input>
{
	friend class Singleton<Input>;
private:
	Input();
public:
	bool					Init();
	bool					Frame();
	bool					Render();
	bool					Release();
public:
	void					MSGEvent(MSG);
public:
	bool					isPressAnyKey();
public:
	KEYSTATE				GetKey(DWORD);
	KEYSTATE				GetMouse(DWORD);
	POINT					GetMousePos();
public:
	void					setKey(DWORD);
private:
	KEYSTATE				KeyCheck(DWORD);
private:
	TCHAR					m_csBuffer[256];
	POINT					m_MousePos;
	KEYSTATE				m_KeyState[256];
	KEYSTATE				m_BeforeMouseState[3];
	KEYSTATE				m_CurrentMouseState[3];
};

#define S_Input Input::getInstance()