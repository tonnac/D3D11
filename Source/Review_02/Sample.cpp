#include "Sample.h"

bool Sample::Init()
{
	m_object.InitSet(m_pDevice, L"Kaho", L"../../data/png/kaho.png", L"vertexshader.txt");
	return true;
}
bool Sample::Frame()
{
	m_object.Frame();
	return true;
}
bool Sample::Render()
{
	m_object.Render(m_pContext);
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
