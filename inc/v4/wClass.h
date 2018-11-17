#pragma once

#include "Device.h"

class wClass : public Device
{
protected:
	wClass(HINSTANCE hInstance, UINT Width, UINT Height, const std::tstring& WindowName);

public:
	static wClass*	getWin();

public:
	bool			Run();

	virtual LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	bool			InitMainWindow();

	virtual bool	GameInit();
	virtual bool	GameRun();
	virtual bool	GameRelease();

	float			AspectRatio();

private:
	void			CenterWindow(const RECT& rt);

protected:
	static wClass*	g_Window;

	bool m_Paused = false;
	bool m_Minimized = false;
	bool m_Maximized = false;
	bool m_Resizing = false;
	bool m_FullscreenState = false;

	std::tstring m_WindowName;
};