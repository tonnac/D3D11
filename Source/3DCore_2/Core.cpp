#include "Core.h"

Core::Core()
{}

bool Core::GameInit()
{
	PreInit();

	CreateDevice();
	CreateFactory();
	CreateSwapChain();
	SetRenderTargetView();
	SetViewPort();

	IDXGIFactory * pFactory = getDXGIFactory();
#ifdef DEVICE_INFO
	Enumeration::CreateDeviceInfo(pFactory);
#endif
	//Alt+Enter 와 윈도우 메시지 막음
	UINT Flags = DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER;
	pFactory->MakeWindowAssociation(g_hWnd, Flags);
	S_Write.Init(getSwapChainPtr());
	S_Input.Init();
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
	CleanupDevice();
	S_Write.Release();
	m_Timer.Release();
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
	S_Input.Frame();
	m_Timer.Frame();
	Frame();
	S_Input.CopytoBefore();
	return true;
}
bool Core::GameRender()
{
	if (PreRender() == false) return false;
	Render();
	S_Input.Render();
	m_Timer.Render();
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
	S_Write.Begin();
	return true;
}
bool Core::PostRender()
{
	S_Write.End();
	IDXGISwapChain* pSwapChain = getSwapChain();
	pSwapChain->Present(0, 0);
	return true;
}

void Core::ResizeDiscard()
{
	S_Write.ResizeDiscard();
}
void Core::ResizeCreate()
{
	S_Write.ResizeCreate();
}