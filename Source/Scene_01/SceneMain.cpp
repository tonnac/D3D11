#include "Core.h"
#include "SceneMgr.h"
#include "mSound.h"

class Sample : public Core
{
public:
	Sample()
	{}
public:
	bool Init()
	{
		S_SceneMgr.Init();
		return true;
	}
	bool Frame()
	{
		S_SceneMgr.Frame();
		return true;
	}
	bool Render()
	{
		S_SceneMgr.Render();
		return true;
	}
	bool Release()
	{
		S_SceneMgr.Release();
		return true;
	}
private:
};

int WINAPI wWinMain(HINSTANCE hinst, HINSTANCE previnst, LPWSTR szCmdLine, int nCmdShow)
{
	Sample wd;
	wd.SetWindow(hinst, 960, 720);
	wd.Run();

	return 0;
}