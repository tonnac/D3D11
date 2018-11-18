#include "wClass.h"

HINSTANCE		g_hInstance = nullptr;
HWND			g_hWnd = nullptr;
int				g_ClientWidth = 0;
int				g_ClientHeight = 0;
wClass* wClass::g_Window = nullptr;

LRESULT CALLBACK 
MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	assert(wClass::getWin() != nullptr);
	return wClass::getWin()->MsgProc(hWnd, msg, wParam, lParam);
}

wClass::wClass(HINSTANCE hInstance, UINT Width, UINT Height, const std::tstring& WindowName)
	:m_WindowName(WindowName)
{
	assert(g_Window == nullptr);
	g_Window = this;
	g_ClientWidth = Width;
	g_ClientHeight = Height;
	g_hInstance = hInstance;
}

wClass* wClass::getWin()
{
	assert(g_Window);
	return g_Window;
}

bool wClass::InitMainWindow()
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;
	wc.hInstance = g_hInstance;
	wc.hIcon = LoadIcon(nullptr, IDI_SHIELD);
	wc.lpszMenuName = m_WindowName.c_str();
	wc.lpszClassName = L"MainWnd";
	wc.hIconSm = LoadIcon(nullptr, IDI_SHIELD);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(nullptr, L"Register Failed.", nullptr, MB_OK);
		return false;
	}

	RECT r = { 0,0,g_ClientWidth,g_ClientHeight };
	AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, FALSE);

	g_hWnd = CreateWindowEx(WS_EX_APPWINDOW, wc.lpszClassName, m_WindowName.c_str(), WS_OVERLAPPEDWINDOW,
		0, 0, r.right - r.left, r.bottom - r.top, 0, 0, g_hInstance, 0);
	assert(g_hWnd != nullptr);

	CenterWindow(r);
	ShowWindow(g_hWnd, SW_SHOW);
	UpdateWindow(g_hWnd);
	return true;
}

int wClass::Run()
{
	GameInit();
	MSG MsgEvent;
	ZeroMemory(&MsgEvent, sizeof(MSG));

	while (MsgEvent.message != WM_QUIT)
	{
		if (PeekMessage(&MsgEvent, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&MsgEvent);
			DispatchMessage(&MsgEvent);
		}
		else
		{
			if (m_Paused == false)
			{
				GameRun();
			}
			else
			{
				Sleep(100);
			}
		}
	}
	return Casting(int, MsgEvent.wParam);
}

LRESULT wClass::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

bool wClass::GameInit()
{
	return true;
}

bool wClass::GameRun()
{
	return true;
}

float wClass::AspectRatio()
{
	return Casting(float, g_ClientWidth) / Casting(float, g_ClientHeight);
}

void wClass::CenterWindow(const RECT& rt)
{
	int iScreenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	int iScreenHeight = GetSystemMetrics(SM_CYFULLSCREEN);
	int x = (iScreenWidth - (g_ClientWidth)) / 2;
	int y = (iScreenHeight - (g_ClientHeight)) / 2;
	MoveWindow(g_hWnd, x, y, rt.right - rt.left, rt.bottom - rt.top, TRUE);
}