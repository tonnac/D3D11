#pragma once

#include "Device.h"

class wClass : public Device
{
	friend LRESULT CALLBACK	MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
protected:
	wClass(HINSTANCE hInstance, UINT Width, UINT Height, const std::tstring& WindowName);
	wClass() = default;

public:
	int				Run();

protected:
	static wClass*	getWin();

	virtual LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	bool			InitMainWindow();

	virtual bool	GameInit();
	virtual bool	GameRun();

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