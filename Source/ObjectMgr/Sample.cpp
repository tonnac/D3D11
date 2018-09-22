#include "Sample.h"
#include "DirectInput.h"

bool Sample::Init()
{
	Background * pBackGround = new Background;
	pBackGround->SetPos(220.0f, 0.0f, 896.0f, 240.0f);
	pBackGround->InitSet(m_pDevice, L"DD", L"../../momodora/data/map/Map.png", L"VertexShader.txt");
	S_Object.AddBackGround(pBackGround);
	Terrain * terrain = new Terrain;
	terrain->SetPos({ 0.0f, 192.0f, 545.0f, 240.0f });
	terrain->InitSet(m_pDevice, L"DD1", L"VertexShader.txt", "VS", "TerrainPS");
	S_Object.AddTerrain(terrain);
	terrain = new Terrain;
	terrain->SetPos({ 545.0f, 175.0f, 545.0f + 83.0f, 175.0 + 65.0f });
	terrain->InitSet(m_pDevice, L"DD1", L"VertexShader.txt", "VS", "TerrainPS");
	S_Object.AddTerrain(terrain);

	return true;
}
bool Sample::Frame()
{
	S_Object.Frame();
	return true;
}
bool Sample::Render()
{
	S_Object.Render(m_pContext);
	return true;
}
bool Sample::Release()
{
	S_Object.Release();
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
}
