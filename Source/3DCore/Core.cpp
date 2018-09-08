#include "Core.h"
#include "Enumeration.h"

bool Core::GameInit()
{
#pragma region
	if (FAILED(CreateDevice())) return false;
	if (FAILED(CreateGIFactory())) return false;
	if (FAILED(CreateSwapChain(g_rtClient.right - g_rtClient.left, g_rtClient.bottom - g_rtClient.top)))
		return false;
	if (FAILED(SetRenderTargetView())) return false;
	SetViewPort();
#pragma endregion Device_Init

#ifdef DEVICE_INFO
	Enumeration::CreateDeviceInfo(getGIFactory());
#endif
	
#pragma region 
	m_DirectWrite.Init();
	IDXGISwapChain * pSwapChain = getSwapChain();
	IDXGISurface* pSurface = nullptr;
	pSwapChain->GetBuffer(0, __uuidof(IDXGISurface), (LPVOID*)&pSurface);
	m_DirectWrite.Set(pSurface);
	pSurface->Release();
#pragma endregion DirectWrite_Init

	IDXGIFactory * pFactory = getGIFactory();
	pFactory->MakeWindowAssociation(g_hWnd, DXGI_MWA_NO_ALT_ENTER | DXGI_MWA_NO_WINDOW_CHANGES);
	m_Timer.Init();
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
	m_Timer.Release();
	m_DirectWrite.Release();
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
	if (CleanupDevice() == false) return false;
	return true;
}
void Core::DeleteDeviceResources()
{
	m_DirectWrite.DiscardDeviceResources();
}
HRESULT	Core::CreateDeviceResources()
{
	IDXGISwapChain * pSwapChain = getSwapChain();
	IDXGISurface * pSurface = nullptr;
	pSwapChain->GetBuffer(0, __uuidof(pSurface), (LPVOID*)&pSurface);
	HRESULT hr = m_DirectWrite.CreateDeviceResources(pSurface);
	pSurface->Release();
	return hr;
}
bool Core::GameFrame()
{
	m_Timer.Frame();
	Frame();
	return true;
}
bool Core::GameRender()
{
	if (GamePreRender() == false) return false;
	m_Timer.Render();

	std::tstring frame = m_Timer.getBuffer();
	D2D1_RECT_F pe = D2D1::RectF(0, 0, static_cast<FLOAT>(g_rtClient.right), static_cast<FLOAT>(g_rtClient.bottom));
	m_DirectWrite.DrawText(pe, frame.c_str(), D2D1::ColorF::YellowGreen);

	Render();
	if (GamePostRender() == false) return false;
	return true;
}
bool Core::GamePreRender()
{
	ID3D11DeviceContext* pContext = getImmediContext();
	ID3D11RenderTargetView* pRenderTarget = getRenderTargetView();
	float ClearColor[] = { 0.12f,0.32f,0.346f,1.0f };
	pContext->ClearRenderTargetView(pRenderTarget, ClearColor);
	m_DirectWrite.Begin();
	return true;
}
bool Core::GamePostRender()
{
	m_DirectWrite.End();
	IDXGISwapChain*	pSwapChain = getSwapChain();
	pSwapChain->Present(0, 0);
	return true;
}