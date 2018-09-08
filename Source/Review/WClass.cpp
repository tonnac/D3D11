#include "WClass.h"

HINSTANCE g_hInstance = nullptr;
HWND	  g_hWnd = nullptr;
WClass*	  g_pWindow = nullptr;
RECT	  g_rtClient;

LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT msg, WPARAM Wparam, LPARAM LParam)
{
	assert(g_pWindow != nullptr);
	return g_pWindow->WndProc(hwnd, msg, Wparam, LParam);
}

WClass::WClass() : m_hWnd(nullptr), m_hInstance(nullptr)
{
	g_pWindow = this;
}
bool WClass::Set(HINSTANCE hInstance, const LONG& iWidth, const LONG& iHeight, const TCHAR* winName)
{
	WNDCLASSEX wndclass;
	ZeroMemory(&wndclass, sizeof(WNDCLASSEX));
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = StaticWndProc;
	wndclass.hInstance = m_hInstance = g_hInstance = hInstance;
	wndclass.lpszMenuName = winName;
	wndclass.lpszClassName = L"Wndclass";

	if (!RegisterClassEx(&wndclass))
	{
		return false;
	}

	RECT rt = { 0, 0, CASTING(LONG,iWidth), CASTING(LONG,iHeight) };

	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, FALSE);

	m_hWnd = g_hWnd = CreateWindowEx(WS_EX_APPWINDOW,
		wndclass.lpszClassName,
		wndclass.lpszMenuName,
		WS_OVERLAPPEDWINDOW,
		0, 0,
		rt.right - rt.left,
		rt.bottom - rt.top, 
		nullptr,
		nullptr,
		wndclass.hInstance,
		nullptr);

	assert(m_hWnd != nullptr);

	GetWindowRect(m_hWnd, &m_rtWindow);
	GetClientRect(m_hWnd, &m_rtClient);

	g_rtClient = m_rtClient;

	CenterWindow();

	ShowWindow(m_hWnd, SW_SHOW);
	return true;
}
bool WClass::Run()
{
	if (GameInit() == false) return false;

	MSG msgEvent;
	ZeroMemory(&msgEvent, sizeof(MSG));

	while (msgEvent.message != WM_QUIT)
	{
		if (PeekMessage(&msgEvent, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msgEvent);
			DispatchMessage(&msgEvent);
		}
		else
		{
			GameRun();
		}
	}
	if (GameRelease() == false) return false;
	return true;
}
LRESULT CALLBACK WClass::WndProc(HWND hWnd, UINT msg, WPARAM WParam, LPARAM LParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
	{
		IDXGISwapChain * pSwapChain = getSwapChain();
		switch (WParam)
		{
			if (pSwapChain == nullptr) break;
			case '0':
			{
				BOOL isFullScreen = FALSE;
				pSwapChain->GetFullscreenState(&isFullScreen, nullptr);
				pSwapChain->SetFullscreenState(!isFullScreen, nullptr);
				if (isFullScreen == TRUE)
				{
					ShowWindow(m_hWnd, SW_SHOW);
				}
				break;
			}
			case '1':
			{
				DXGI_MODE_DESC ModeDesc;
				ZeroMemory(&ModeDesc, sizeof(DXGI_MODE_DESC));
				ModeDesc.RefreshRate.Numerator = 60;
				ModeDesc.RefreshRate.Denominator = 1;
				ModeDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				ModeDesc.Width = 800;
				ModeDesc.Height = 600;
				pSwapChain->ResizeTarget(&ModeDesc);
				break;
			}
			case '2':
			{
				DXGI_MODE_DESC ModeDesc;
				ZeroMemory(&ModeDesc, sizeof(DXGI_MODE_DESC));
				ModeDesc.RefreshRate.Numerator = 60;
				ModeDesc.RefreshRate.Denominator = 1;
				ModeDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				ModeDesc.Width = 1024;
				ModeDesc.Height = 768;
				pSwapChain->ResizeTarget(&ModeDesc);
				break;
			}	
			case VK_ESCAPE:
			{
				PostQuitMessage(0);
				break;
			}
		}
		break;
	}
	case WM_SIZE:
	{
		if (WParam != SIZE_MINIMIZED)
		{
			UINT iWidth = LOWORD(LParam);
			UINT iHeight = HIWORD(LParam);
			ResizeDevice(iWidth, iHeight);
			m_rtClient = g_rtClient;
			GetWindowRect(m_hWnd, &m_rtWindow);
		}
		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	}
	return DefWindowProc(hWnd, msg, WParam, LParam);
}

bool WClass::GameInit()
{
	return true;
}
bool WClass::GameRun()
{
	return true;
}
bool WClass::GameRelease()
{
	return true;
}

void WClass::CenterWindow()
{
	int iScreenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	int iScreenHeight = GetSystemMetrics(SM_CYFULLSCREEN);

	int x = (iScreenWidth - (m_rtClient.right - m_rtClient.left)) / 2;
	int y = (iScreenHeight - (m_rtClient.bottom - m_rtClient.top)) / 2;

	MoveWindow(m_hWnd, x, y, m_rtClient.right - m_rtClient.left, m_rtClient.bottom - m_rtClient.top, TRUE);
}