#pragma once
#include "Device.h"

class Window : public Device
{
public:
	Window();
public:
	bool			 InitWindow(HINSTANCE hInstance, UINT width , UINT height 
						,int nCmdShow, const TCHAR* strWindowTitle);
	void			 CenterWindow();
public:				 
	virtual bool	 GameInit();
	virtual bool	 GameRun();
	virtual bool	 GameRelease();
	virtual bool	 Run();
	virtual LRESULT  WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
private:
	HINSTANCE		 m_hInstance;
	HWND	 		 m_hWnd;
	RECT	 		 m_rtClient;
	RECT	 		 m_rtWindow;
	UINT	 		 m_iWindowWidth;
	UINT	 		 m_iWindowHeight;
	DWORD			 m_dwStyle;
};