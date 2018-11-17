
#include "Core.h"
#include "DirectWrite.h"
#include "DirectInput.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;
using namespace std;
using namespace Input;

Core::Core(HINSTANCE hInstance, UINT Width, UINT Height, const std::tstring& WindowName) 
	: wClass(hInstance, Width, Height, WindowName)
{
}

bool Core::Initialize()
{
	if (!InitMainWindow())
		return false;
	if (!InitDirect3D())
		return false;
	if (!S_Write.Initialize())
		return false;

	OnResize();
	return true;
}

bool Core::GameInit()
{
	m_Timer.Reset();
	S_Input.Init();
	Init();
	return true;
}

bool Core::GameRun()
{
	GameFrame();
	GameRender();
	return true;
}

bool Core::GameRelease()
{
	Release();
	return true;
}

LRESULT Core::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			m_Paused = true;
			m_Timer.Stop();
		}
		else
		{
			m_Paused = false;
			m_Timer.Start();
		}
		return 0;

	case WM_SIZE:
		g_ClientWidth = LOWORD(lParam);
		g_ClientHeight = HIWORD(lParam);
		if (m_pd3dDevice != nullptr)
		{
			if (wParam == SIZE_MINIMIZED)
			{
				m_Paused = true;
				m_Minimized = true;
				m_Maximized = false;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				m_Paused = false;
				m_Minimized = false;
				m_Maximized = true;
				OnResize();
			}
			else if (wParam == SIZE_RESTORED)
			{
				if (m_Minimized == true)
				{
					m_Paused = false;
					m_Minimized = false;
					OnResize();
				}
				else if (m_Maximized == true)
				{
					m_Paused = false;
					m_Maximized = false;
					OnResize();
				}
				else if (m_Resizing == true)
				{

				}
				else
				{
					OnResize();
				}
			}
		}
		return 0;

	case WM_ENTERSIZEMOVE:
		m_Paused = true;
		m_Resizing = true;
		m_Timer.Stop();
		return 0;

	case WM_EXITSIZEMOVE:
		m_Paused = false;
		m_Resizing = false;
		m_Timer.Start();
		OnResize();
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_MENUCHAR:
		return MAKELRESULT(0, MNC_CLOSE);

	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
		return 0;

	case WM_KEYUP:
		if (wParam == VK_ESCAPE)
		{
			PostQuitMessage(0);
		}
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
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

void Core::CalculateFrame()
{
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;
	m_Timer.Tick();
	++frameCnt;

	if (m_Timer.TotalTime() - timeElapsed >= 1.0f)
	{
		float fps = Casting(float, frameCnt);
		float mspf = 1000.0f / fps;

		tstring fpsStr = to_tstring(fps);
		tstring mspfStr = to_tstring(mspf);

		m_FrameInfo = L"fps:" + fpsStr + L"    mspf:" + mspfStr;

		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}

void Core::OnResize()
{
	S_Write.Reset();
	Device::OnResize();
	S_Write.OnResize(m_pSwapchain.Get());
}

bool Core::GameFrame()
{
	CalculateFrame();
	S_Input.Frame();
	if (S_Input.getKeyState(DIK_LCONTROL) == KEYSTATE::KEY_HOLD && S_Input.getKeyState(DIK_F) == KEYSTATE::KEY_PUSH)
	{
		m_bFrameinfo = !m_bFrameinfo;
	}
	Frame();
	S_Input.PostFrame();
	return true;
}

bool Core::PreRender()
{
	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(), Colors::PaleGreen);
	m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(), 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_pImmediateContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
	m_pImmediateContext->RSSetViewports(1, &m_Viewport);
	S_Write.Begin();

	if(m_bFrameinfo)
		S_Write.DrawText({ 0, 0, 800, 600 }, m_FrameInfo, Colors::Black);
	return true;
}

bool Core::GameRender()
{
	PreRender();
	{
		Render();
	}
	PostRender();
	return true;
}

bool Core::PostRender()
{
	S_Write.End();
	ThrowifFailed(m_pSwapchain->Present(0, 0));
	return true;
}

