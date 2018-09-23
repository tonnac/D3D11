#include "wClass.h"
#include "DirectInput.h"

HINSTANCE		g_hInstance = nullptr;
HWND			g_hWnd = nullptr;
RECT			g_rtClient;
wClass* wClass::g_Window = nullptr;
bool			g_bActive = false;
FLOAT			g_fImageWidth = 0.0f;
FLOAT			g_fImageHeight = 0.0f;

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	assert(wClass::getWin() != nullptr);
	return wClass::getWin()->MsgProc(hWnd, msg, wParam, lParam);
}

wClass::wClass()
{
	assert(g_Window == nullptr);
	g_Window = this;
}
wClass* wClass::getWin()
{
	GETPTR(g_Window);
}
bool wClass::Set(HINSTANCE hInstance, const LONG& Width, const LONG& Height, const std::tstring& WindowName)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;
	wc.hInstance = g_hInstance = hInstance;
	wc.hIcon = LoadIcon(nullptr, IDI_SHIELD);
	wc.lpszMenuName = WindowName.c_str();
	wc.lpszClassName = L"MainWnd";
	wc.hIconSm = LoadIcon(nullptr, IDI_SHIELD);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(nullptr, L"Register Failed.", nullptr, MB_OK);
		return false;
	}

	RECT r = { 0,0,Width,Height };
	AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, FALSE);

	g_hWnd = CreateWindowEx(WS_EX_APPWINDOW, wc.lpszClassName, WindowName.c_str(), WS_OVERLAPPEDWINDOW,
		0, 0, r.right - r.left, r.bottom - r.top, 0, 0, g_hInstance, 0);
	assert(g_hWnd != nullptr);

	GetWindowRect(g_hWnd, &m_rtWindow);
	GetClientRect(g_hWnd, &g_rtClient);

	g_fImageWidth = g_rtClient.right / 3.0f;
	g_fImageHeight = g_rtClient.bottom / 3.0f;

	CenterWindow(r);
	ShowWindow(g_hWnd, SW_SHOW);
	UpdateWindow(g_hWnd);
	return true;
}
bool wClass::Run()
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
			GameRun();
		}
	}
	return GameRelease();
}
LRESULT wClass::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
			case '2':
			{
				DXGI_MODE_DESC modeDesc;
				ZeroMemory(&modeDesc, sizeof(modeDesc));
				modeDesc.Width = 1024;
				modeDesc.Height = 768;
				modeDesc.RefreshRate.Numerator = 60;
				modeDesc.RefreshRate.Denominator = 1;
				modeDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				m_pSwapChain->ResizeTarget(&modeDesc);
				break;
			}
			case '1':
			{
				DXGI_MODE_DESC modeDesc;
				ZeroMemory(&modeDesc, sizeof(modeDesc));
				modeDesc.Width = 800;
				modeDesc.Height = 600;
				modeDesc.RefreshRate.Numerator = 60;
				modeDesc.RefreshRate.Denominator = 1;
				modeDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				m_pSwapChain->ResizeTarget(&modeDesc);
				break;
			}
			case '0':
			{
				BOOL isScreenMode = FALSE;
				m_pSwapChain->GetFullscreenState(&isScreenMode, nullptr);
				m_pSwapChain->SetFullscreenState(!isScreenMode, nullptr);
				if (!isScreenMode)
				{
					ShowWindow(g_hWnd, SW_SHOW);
				}
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
	case WM_ACTIVATE:
	{
		if (LOWORD(wParam) == WA_INACTIVE)
			g_bActive = false;
		else
			g_bActive = true;
		S_Input.SetAcquire(g_bActive);
		break;
	}
	case WM_SIZE:
	{
		if (wParam != SIZE_MINIMIZED)
		{
			UINT iWidth = LOWORD(lParam);
			UINT iHeight = HIWORD(lParam);
			GetClientRect(g_hWnd, &g_rtClient);
			GetWindowRect(g_hWnd, &m_rtWindow);
			ResizeDevice(iWidth, iHeight);
		}
		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
	
}
bool wClass::GameInit()
{
	return true;
}
bool wClass::GameRun()
{
	return true;
}
bool wClass::GameRelease()
{
	return true;
}
void wClass::CenterWindow(const RECT& rt)
{
	int iScreenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	int iScreenHeight = GetSystemMetrics(SM_CYFULLSCREEN);
	int x = (iScreenWidth - (g_rtClient.right - g_rtClient.left)) / 2;
	int y = (iScreenHeight - (g_rtClient.bottom - g_rtClient.top)) / 2;
	MoveWindow(g_hWnd, x, y, rt.right - rt.left, rt.bottom - rt.top, TRUE);
}