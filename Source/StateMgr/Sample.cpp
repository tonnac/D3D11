#include "Sample.h"
#include "DirectInput.h"

bool Sample::Init()
{
	S_Scene.Initialize(m_pDevice, m_pContext);
	// 25 36
	m_Character.SetPos(m_pDevice, { 100.0f,500.0f });
	m_Character.InitSet(m_pDevice, L"Kaho", Filepath::m_Pngpath[L"Kaho"], Filepath::m_Txtpath[L"Shader"]);
	return true;
}
bool Sample::Frame()
{
	S_Scene.Frame();
	m_Character.Frame();
	return true;
}
bool Sample::Render()
{
	S_Scene.Render();
	m_Character.Render(m_pContext);
	return true;
}
bool Sample::Release()
{
	S_Scene.Release();
	m_Character.Release();
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
