#pragma once
#include "Device.h"

class wClass : public Device
{
public:
	wClass();
public:
	static wClass*	getWin();
public:
	bool			Set(HINSTANCE hInstance, const LONG& Width, const LONG& Height, const std::tstring& WindowName);
	bool			Run();
public:
	virtual LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	virtual bool	GameInit();
	virtual bool	GameRun();
	virtual bool	GameRelease();
private:
	void			CenterWindow(const RECT& rt);
protected:
	static wClass*	g_Window;
	RECT			m_rtWindow;
};