#include "Sample.h"

bool Sample::Init()
{
	S_Scene.Initialize(m_pDevice, m_pContext);
	m_Ladder = new Ladder;
	m_Ladder->SetPos({536.0f,100.0f,551.0f,208.0f});
	m_Ladder->InitSet(m_pDevice, L"Terrain", Filepath::m_Txtpath[L"Shader"], "VS", "TerrainPS");
	S_Object.AddTerrain(m_Ladder);
	return true;
}
bool Sample::Frame()
{
	S_Scene.Frame();
	return true;
}
bool Sample::Render()
{
	S_Scene.Render();
	return true;
}
bool Sample::Release()
{
	S_Scene.Release();
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
