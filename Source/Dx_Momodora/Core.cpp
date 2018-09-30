#include "Core.h"
#include "Texture.h"
#include "Shader.h"
#include "Sprite.h"

D3DXMATRIX	g_mToProj;
bool		g_DebugMode = false;

bool Core::GameInit()
{
	DeviceInit();
	PreInit();
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
	S_Sprite.Release();
	S_Texture.Release();
	S_Shader.Release();
	S_Write.Release();
	S_Input.Release();
	S_Sound.Release();
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
void Core::ResizeDevice(const LONG& Width, const LONG& Height)
{
	Device::ResizeDevice(Width, Height);
	MatrixInit();
}
bool Core::DeviceInit()
{
#ifdef DEVICE_INFO
	DeviceInfo::CreateDeviceInfo();
#endif
	InitDevice();
	S_Input.Init();
	S_Write.Set(m_pSwapChain);
	m_Timer.Init();
	return true;
}
bool Core::PreInit()
{
	MatrixInit();
	Filepath::Init(L"../../momodora/data/txt/Filepath.txt");
	S_Sprite.SpriteSet(Filepath::m_Txtpath[L"KahoSprite"]);
	S_Sound.Init();
	S_Sound.LoadFile(Filepath::m_Txtpath[L"Snd"]);
	return true;
}
bool Core::GameFrame()
{
	S_Input.Frame();
	S_Sound.Frame();
	if (S_Input.getKeyState(DIK_DELETE) == Input::KEYSTATE::KEY_PUSH)
	{
		g_DebugMode = !g_DebugMode;
	}
	m_Timer.Frame();
	Frame();
	S_Input.PostFrame();
	return true;
}
bool Core::PreRender()
{
	float fColor[] = { 0.0f ,0.0f, 0.0f, 1.0f };
	m_pContext->ClearRenderTargetView(m_pRenderTargetView, fColor);
	S_Write.Begin();
	return true;
}
bool Core::GameRender()
{
	PreRender();
	{
		S_Input.Render();
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
bool Core::MatrixInit()
{
	D3DXMatrixIdentity(&g_mToProj);
	g_mToProj._11 = 2.0f / g_rtClient.right;
	g_mToProj._31 = -1.0f;
	g_mToProj._22 = -2.0f / g_rtClient.bottom;
	g_mToProj._32 = 1.0f;
	return true;
}