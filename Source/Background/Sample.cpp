#include "Sample.h"

bool Sample::Init()
{
	m_Background.SetPos({ g_rtClient.right * 0.5f, g_rtClient.bottom * 0.5f }, { 222.0f, 0.0f, 222.0f + 406.0f, 240.0f });
	m_Background.InitSet(m_pDevice, L"DD", L"../../momodora/data/map/Map1.png", L"VertexShader.txt");
	return true;
}
bool Sample::Frame()
{
	m_Background.Frame();
	return true;
}
bool Sample::Render()
{
	m_Background.Render(m_pContext);
	return true;
}
bool Sample::Release()
{
	m_Background.Release();
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
