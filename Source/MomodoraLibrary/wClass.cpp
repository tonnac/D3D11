#include "wClass.h"

HWND				 g_hWnd = nullptr;
HINSTANCE            g_hInstance = nullptr;
RECT				 g_rtClient;
bool				 g_bActiveApp = false;

LRESULT	CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_ACTIVATEAPP:
		g_bActiveApp = (BOOL)wparam;
		break;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

Window::Window()
{

}

bool Window::SetWindow(HINSTANCE hinst, LONG lWidth, LONG lHeight)
{
	WNDCLASSEX wndclass;
	ZeroMemory(&wndclass, sizeof(WNDCLASSEX));
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = g_hInstance = hinst;
	wndclass.lpszClassName = L"Class";
	wndclass.lpszMenuName = L"Momodora: Reverie Under the Moonlight";

	if (!RegisterClassEx(&wndclass))
	{
		return false;
	}


	m_rtClient.right = lWidth;
	m_rtClient.bottom = lHeight;

	AdjustWindowRect(&m_rtClient, WS_OVERLAPPEDWINDOW, false);

	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW,
		wndclass.lpszClassName,
		wndclass.lpszMenuName,
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		m_rtClient.right - m_rtClient.left,
		m_rtClient.bottom - m_rtClient.top,
		nullptr,
		nullptr,
		wndclass.hInstance,
		nullptr);

	assert(m_hWnd != nullptr);

	g_hWnd = m_hWnd;

	GetWindowRect(m_hWnd, &m_rtWindow);
	GetClientRect(m_hWnd, &m_rtClient);

	g_rtClient = m_rtClient;

	CenterWindow();
	return true;
}
bool Window::Run()
{
	if (!GameInit()) return false;
	ShowWindow(m_hWnd, SW_SHOW);

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			S_Input.MSGEvent(msg);
		}
		else
		{
			GameFrame();
			GameRender();
		}
	}
	return GameRelease();
}
bool Window::Init()
{
	return true;
}
bool Window::Frame()
{
	return true;
}
bool Window::Render()
{
	return true;
}
bool Window::Release()
{
	return true;
}
bool Window::GameInit()
{
	return true;
}
bool Window::GameFrame()
{
	return true;
}
bool Window::GameRender()
{
	return true;
}
bool Window::GameRelease()
{
	return true;
}


void Window::CenterWindow()
{
	int iScreenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	int iScreenHeight = GetSystemMetrics(SM_CYFULLSCREEN);
	int x = (iScreenWidth - (m_rtWindow.right - m_rtWindow.left)) / 2;
	int y = (iScreenHeight - (m_rtWindow.bottom - m_rtWindow.top)) / 2;
	MoveWindow(m_hWnd, x, y, m_rtWindow.right - m_rtWindow.left, m_rtWindow.bottom - m_rtWindow.top, true);
}