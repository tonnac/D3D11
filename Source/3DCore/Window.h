#pragma once
#include "Device.h"

class Window : public Device
{
public:
	Window();
public:
	bool			 InitWindow(HINSTANCE hInstance, const UINT& width , const UINT& height 
						,const int& nCmdShow, const TCHAR* strWindowTitle);
	void			 CenterWindow();
public:				 
	virtual bool	 GameInit();
	virtual bool	 GameRun();
	virtual bool	 GameRelease();
	virtual bool	 Run();
	virtual LRESULT  WndProc(HWND hwnd, const UINT& msg, WPARAM wparam, LPARAM lparam);
private:
	HINSTANCE		 m_hInstance;
	HWND	 		 m_hWnd;
	RECT	 		 m_rtClient;
	RECT	 		 m_rtWindow;
	UINT	 		 m_iWindowWidth;
	UINT	 		 m_iWindowHeight;
	DWORD			 m_dwStyle;
};