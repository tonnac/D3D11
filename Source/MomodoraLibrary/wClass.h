#pragma once
#include "Std.h"
#include "Input.h"

LRESULT	CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

class Window
{
public:
	Window();
public:
	bool					SetWindow(HINSTANCE, LONG, LONG);
	bool					Run();
public:
	virtual bool			GameInit();
	virtual bool			GameFrame();
	virtual bool			GameRender();
	virtual bool			GameRelease();
	virtual bool			Init();
	virtual bool			Frame();
	virtual bool			Render();
	virtual bool			Release();
private:
	void					CenterWindow();
private:
	HINSTANCE				m_hInstance;
	RECT					m_rtClient;
	RECT					m_rtWindow;
	HWND					m_hWnd;
};