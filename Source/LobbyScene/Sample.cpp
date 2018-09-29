#include "Sample.h"

bool Sample::Init()
{
//	S_Scene.Initialize(m_pDevice, m_pContext);
	m_Lobby.InitSet(m_pDevice, L"Basic", Filepath::m_Pngpath[L"Lobby"], Filepath::m_Txtpath[L"Shader"]);
	return true;
}
bool Sample::Frame()
{
//	S_Scene.Frame();
	m_Lobby.Frame();
	return true;
}
bool Sample::Render()
{
//	S_Scene.Render();
	m_Lobby.Render(m_pContext);
	return true;
}
bool Sample::Release()
{
//	S_Scene.Release();
	m_Lobby.Release();
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
