#include "Sample.h"
#include "DirectInput.h"

bool Sample::Init()
{
	m_GameScene.SendDevice(m_pDevice, m_pContext);
	m_GameScene.Init();
//	m_Sb.Initialize(L"VertexShad1er.txt");
	return true;
}
bool Sample::Frame()
{
	m_GameScene.Frame();
	return true;
}
bool Sample::Render()
{
	m_GameScene.Render();
	return true;
}
bool Sample::Release()
{
	m_GameScene.Release();
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
