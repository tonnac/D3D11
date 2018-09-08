#include "Sample.h"



int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevhInstance, LPWSTR szCmdLine, int nCmdShow)
{
	Sample sd;
	sd.Set(hInstance, 800, 600, L"Sample");
	sd.Run();
	return 0;
}