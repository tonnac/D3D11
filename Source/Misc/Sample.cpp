#include "Sample.h"

bool Sample::Init()
{
	S_Scene.Initialize(m_pDevice, m_pContext);
	
	return true;
}
bool Sample::Frame()
{
	S_Scene.Frame();
	m_Misc.Frame();
	return true;
}
bool Sample::Render()
{
	S_Scene.Render();
	m_Misc.Render(m_pContext);
	return true;
}
bool Sample::Release()
{
	S_Scene.Release();
	m_Misc.Release();
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
