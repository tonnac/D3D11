#include "Core.h"

bool Core::GameInit()
{
	PreInit();

	if (FAILED(CreateDevice())) return false;
	if (FAILED(CreateFactory())) return false;
	if (FAILED(CreateSwapChain())) return false;
	if (FAILED(SetRenderTargetView())) return false;
	SetViewPort();

	IDXGIFactory * pFactory = getDXGIFactory();

	//Alt+Enter 와 윈도우 메시지 막음
	UINT Flags = DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER;
	pFactory->MakeWindowAssociation(g_hWnd, Flags);

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
	CleanupDevice();
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
bool Core::GameFrame()
{
	Frame();
	return true;
}
bool Core::GameRender()
{
	if (PreRender() == false) return false;
	Render();
	if (PostRender() == false) return false;
	return true;
}
bool Core::PreInit()
{
	return true;
}
bool Core::PreRender()
{
	ID3D11DeviceContext* pContext = getContext();
	ID3D11RenderTargetView* pRenderTargetView = getRenderTargetView();
	
	float Color[] = { 0.8f,1.0f,0.8f,1.0f };
	pContext->ClearRenderTargetView(pRenderTargetView, Color);
	return true;
}
bool Core::PostRender()
{
	IDXGISwapChain* pSwapChain = getSwapChain();
	pSwapChain->Present(0, 0);
	return true;
}