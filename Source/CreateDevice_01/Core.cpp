#include "Core.h"

bool Core::GameInit()
{
	if (FAILED(CreateDevice())) return false;
	if (FAILED(CreateGIFactory())) return false;
	if (FAILED(CreateSwapChain(g_rtClient.right - g_rtClient.left, g_rtClient.bottom - g_rtClient.top)))
		return false;
	if (FAILED(SetRenderTargetView())) return false;
	SetViewPort();
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
bool Core::GameFrame()
{
	Frame();
	return true;
}
bool Core::GameRender()
{
	if (GamePreRender() == false) return false;
	Render();
	if (GamePostRender() == false) return false;
	return true;
}
bool Core::GamePreRender()
{
	ID3D11DeviceContext* pContext = getImmediContext();
	ID3D11RenderTargetView* pRenderTarget = getRenderTargetView();
	IDXGISwapChain*	pSwapChain = getSwapChain();
	pContext->OMSetRenderTargets(1, &pRenderTarget, NULL);
	float ClearColor[] = { 1.0f,1.0f,0.0f,1.0f };
	pContext->ClearRenderTargetView(pRenderTarget, ClearColor);
	pSwapChain->Present(0, 0);
	return true;
}
bool Core::GamePostRender()
{
	return true;
}