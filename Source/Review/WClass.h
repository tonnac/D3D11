#pragma once
#include "Device.h"

class WClass : public Device
{
public:
	WClass();
public:
	bool	Set(HINSTANCE hInstance, LONG iWidth, LONG iHeight, const TCHAR* winName);
	bool	Run();
	virtual LRESULT	CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM WParam, LPARAM LParam);
public:
	virtual	bool	GameInit();
	virtual	bool	GameRun();
	virtual	bool	GameRelease();
private:
	void		CenterWindow();
private:
	HINSTANCE	m_hInstance;
	HWND		m_hWnd;
	RECT		m_rtClient;
	RECT		m_rtWindow;
	LONG		m_iWidth;
	LONG		m_iHeight;
};