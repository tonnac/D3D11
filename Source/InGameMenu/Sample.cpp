#include "Sample.h"

bool Sample::Init()
{
	S_Scene.Initialize(m_pDevice, m_pContext);

	m_Menu.InitSet(m_pDevice, L"Terrain", Filepath::m_Pngpath[L"InGameMenu"], Filepath::m_Txtpath[L"Shader"], "VS", "TerrainPS");
	return true;
}
bool Sample::Frame()
{
//	S_Scene.Frame();
	m_Menu.Frame();
	return true;
}
bool Sample::Render()
{
//	S_Scene.Render();
	m_Menu.Render(m_pContext);
	return true;
}
bool Sample::Release()
{
//	S_Scene.Release();
	m_Menu.Release();
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
