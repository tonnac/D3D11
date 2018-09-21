#include "Sample.h"


bool Sample::Init()
{
	m_Maze = m_Builder.CreateMaze(m_pDevice, 3, 3);
	return true;
}
bool Sample::Frame()
{
	m_Maze.Frame();
	return true;
}
bool Sample::Render()
{
	m_Maze.Render(m_pContext);
	return true;
}
bool Sample::Release()
{
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
