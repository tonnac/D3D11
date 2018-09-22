#include "Sample.h"
#include "DirectInput.h"

bool Sample::Init()
{
	m_Background.SetPos(220.0f, 0.0f, 896.0f, 240.0f);
	m_Background.InitSet(m_pDevice, L"DD", L"../../momodora/data/map/Map.png", L"VertexShader.txt");
	Terrain * terrain = new Terrain;
	terrain->SetPos({ 0.0f, 192.0f, 545.0f, 240.0f });
	terrain->InitSet(m_pDevice, L"DD1", L"VertexShader.txt", "VS", "TerrainPS");
	m_TerrainList.push_back(terrain);
	terrain = new Terrain;
	terrain->SetPos({ 545.0f, 175.0f, 545.0f + 83.0f, 175.0 + 65.0f });
	terrain->InitSet(m_pDevice, L"DD1", L"VertexShader.txt", "VS", "TerrainPS");
	m_TerrainList.push_back(terrain);

	return true;
}
bool Sample::Frame()
{
	m_Background.Frame();
	for (auto& it : m_TerrainList)
	{
		it->Frame();
	}
	if (S_Input.getKeyState(DIK_RIGHT) == Input::KEYSTATE::KEY_HOLD)
	{
		m_Background.Scroll(g_fSecPerFrame * 200.0f);
		for (auto& it : m_TerrainList)
		{
			it->Scroll(-g_fSecPerFrame * 600.0f);
		}
	}
	//if (S_Input.getKeyState(DIK_LEFT) == Input::KEYSTATE::KEY_HOLD)
	//{
	//	m_Background.Scroll(-g_fSecPerFrame * 200.0f);
	//	m_Terrain.Scroll(+g_fSecPerFrame * 600.0f);
	//}
	return true;
}
bool Sample::Render()
{
	m_Background.Render(m_pContext);
	if (S_Input.getKeyState(DIK_HOME) == Input::KEYSTATE::KEY_HOLD)
	{
		for (auto& it : m_TerrainList)
		{
			it->Render(m_pContext);
		}
	}
	return true;
}
bool Sample::Release()
{
	m_Background.Release();
	for (auto& it : m_TerrainList)
	{
		it->Release();
	}
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
