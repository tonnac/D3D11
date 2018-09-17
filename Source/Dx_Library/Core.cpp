#include "Core.h"

bool Core::GameInit()
{
#ifdef DEVICE_INFO
	DeviceInfo::CreateDeviceInfo();
#endif
	InitDevice();
	S_Input.Init();
	S_Write.Set(m_pSwapChain);
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
	S_Write.Release();
	S_Input.Release();
	m_Timer.Release();
	Release();
	CleanupDevice();
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
	S_Input.PostFrame();
	return true;
}
bool Core::PreRender()
{
	float fColor[] = { 0.8f ,1.0f, 0.8f, 1.0f };
	m_pContext->ClearRenderTargetView(m_pRenderTargetView, fColor);
	S_Write.Begin();
	return true;
}
bool Core::GameRender()
{
	PreRender();
	{
		m_Timer.Render();
		Render();
	}
	PostRender();
	return true;
}
bool Core::PostRender()
{
	S_Write.End();
	ThrowifFailed(m_pSwapChain->Present(0, 0));
	return true;
}