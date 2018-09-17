#include "Core.h"
#include "mSound.h"
HDC			g_hOffScreenDC = nullptr;
HDC			g_hScreenDC = nullptr;
int			g_inewCount = 0;
MEM_MAP		MemoryMap;

bool Core::GameInit()
{
	FrameShow = false;
//	HBRUSH bkBrush = CreateSolidBrush(RGB(0, 0, 0));
	m_hScreenDC = g_hScreenDC = GetDC(g_hWnd);
	m_hOffScreenDC = g_hOffScreenDC = CreateCompatibleDC(m_hScreenDC);
	HBITMAP hoffbitmap = CreateCompatibleBitmap(m_hScreenDC, g_rtClient.right, g_rtClient.bottom);
	SelectObject(m_hOffScreenDC, hoffbitmap);
//	SelectObject(m_hOffScreenDC, bkBrush);

	m_Timer.Init();
	S_Input.Init();
	S_Sound.Init();
	S_Sound.LoadFile("../../data/txt/Sound.txt");
	Init();
	return true;
}
bool Core::GameFrame()
{
	if (S_Input.GetKey(VK_LCONTROL) == KEYSTATE::KEY_HOLD && S_Input.GetKey('F') == KEYSTATE::KEY_PUSH)
	{
		FrameShow = !FrameShow;
	}
	m_Timer.Frame();
	S_Input.Frame();
	S_Sound.Frame();

	Frame();
	return true;
}
bool Core::GameRender()
{
	if (GamePreRender())
	{
		Render();
		if (FrameShow)
		{
			m_Timer.Render();
			S_Input.Render();
		}
	}
	GamePostRender();
	return true;
}
bool Core::GameRelease()
{
	DeleteDC(m_hOffScreenDC);
	ReleaseDC(g_hWnd, m_hScreenDC);

	S_Sound.Release();
	m_Timer.Release();
	S_Input.Release();
	Release();
	return true;
}
bool Core::Init()
{
	return true;
}
bool Core::Frame()
{
	return true;
}
bool Core::Render()
{
	return true;
}
bool Core::Release()
{
	return true;
}
bool Core::GamePreRender()
{
//	PatBlt(m_hOffScreenDC, 0, 0, g_rtClient.right, g_rtClient.bottom, PATCOPY);
	return true;
}
bool Core::GamePostRender()
{
	BitBlt(m_hScreenDC, 0, 0, g_rtClient.right, g_rtClient.bottom, m_hOffScreenDC, 0, 0, SRCCOPY);
	return true;
}

