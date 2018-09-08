#include "Core.h"

bool Core::GameInit()
{
	PreInit();

	if (FAILED(CreateDevice())) return false;
	if (FAILED(CreateFactory())) return false;
	if (FAILED(CreateSwapChain())) return false;
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