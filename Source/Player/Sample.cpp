#include "Sample.h"

bool Sample::Init()
{
	m_object.Init();
	m_object.SetPos({ g_rtClient.right * 0.5f, g_rtClient.bottom * 0.5f }, { 600.0f,400.0f });
	m_object.InitSet(m_pDevice, L"EE", L"../../data/ui/popup_normal.png", L"VertexShader.txt");
	m_Trie.InitSet(m_pDevice, L"DD", L"../../data/ui/popup_normal.png", L"VertexShader.txt");
	return true;
}
bool Sample::Frame()
{
	m_object.Frame();
	m_Trie.Frame();
	return true;
}
bool Sample::Render()
{
	m_object.Render(m_pContext);
	m_Trie.Render(m_pContext);
	return true;
}
bool Sample::Release()
{
	m_object.Release();
	return true;
}
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInst, LPWSTR szCmdLine, int nCmdShow)
{
	try
	{
		Sample sd;
		sd.Set(hInstance, 800, 600, L"Sample");
		return sd.Run();
	}
	catch (DxException &e)
	{
		MessageBox(nullptr, e.ToString().c_str(), L"FAILED", MB_OK);
		return 0;
	}
}
