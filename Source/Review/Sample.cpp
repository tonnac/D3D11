#include "Sample.h"
#include "DirectInput.h"

bool Sample::Init()
{
	return true;
}
bool Sample::Frame()
{
	return true;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevhInstance, LPWSTR szCmdLine, int nCmdShow)
{
	Sample sd;
	sd.Set(hInstance, 800, 600, L"Sample");
	sd.Run();
	return 0;
}