#include "Sample.h"

Sample::Sample()
{
}

bool Sample::Init()
{
	m_Timer.Init();
	return true;
}
bool Sample::Frame()
{
	m_Timer.Frame();
	return true;
}
bool Sample::Render()
{
	m_Timer.Render();
	return true;
}
bool Sample::Release()
{
	m_Timer.Release();
	return true;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInst, LPWSTR szCmdLine, int nCmdShow)
{
	Sample wd;
	wd.InitWindow(hInstance, 800, 600, nCmdShow, L"FullScreen");
	wd.Run();
	return 0;
}