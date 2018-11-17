#pragma once

#include "wClass.h"
#include "Timer.h"

class Core : public wClass
{
protected:
	Core(HINSTANCE hInstance, UINT Width, UINT Height, const std::tstring& WindowName);
public:
	bool			Initialize();

protected:
	bool			GameInit()override;
	bool			GameRun()override;
	bool			GameRelease()override;
	LRESULT			MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)override;


	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	Render();
	virtual bool	Release();

	void			CalculateFrame();
	void			OnResize()override;

private:
	bool			GameFrame();
	bool			PreRender();
	bool			GameRender();
	bool			PostRender();

protected:
	bool			m_bFrameinfo = true;
	std::tstring	m_FrameInfo;
	Timer			m_Timer;
};