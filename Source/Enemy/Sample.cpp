#include "Sample.h"

bool Sample::Init()
{
	S_Scene.Initialize(m_pDevice, m_pContext);
	m_Enemy.InitSet(m_pDevice, L"Basic", Filepath::m_Pngpath[L"Monkey"], Filepath::m_Txtpath[L"Shader"]);
	m_Enemy.SetCenterPos({ 500.0f,480.0f });
	m_Enemy.setFSM(S_FSMMgr.LoadFSM(L"FSM"));
	D2D1_RECT_F rt = { 554.0f,100.0f,1173.0f,573.0f };
	m_Enemy.setArea(rt);
	return true;
}
bool Sample::Frame()
{
	S_Scene.Frame();
	m_Enemy.Frame();
	return true;
}
bool Sample::Render()
{
	S_Scene.Render();
	m_Enemy.Render(m_pContext);
	return true;
}
bool Sample::Release()
{
	S_Scene.Release();
	m_Enemy.Release();
	return true;
}
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInst, LPWSTR szCmdLine, int nCmdShow)
{
	try
	{
		Sample sd;
		sd.Set(hInstance, 960, 720, L"Sample");
		return sd.Run();
	}
	catch (DxException &e)
	{
		MessageBox(nullptr, e.ToString().c_str(), L"FAILED", MB_OK);
		return 0;
	}
	catch (FileException f)
	{
		return 0;
	}
}
