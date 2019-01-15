
#include "Sample.h"
#include "DirectInput.h"
#include "LightStorage.h"

#include <chrono>

using namespace std::chrono;

using namespace DirectX;

struct Data
{
	XMFLOAT3 v1;
	XMFLOAT2 v2;
};

Sample::Sample(HINSTANCE hInstance, UINT Width, UINT Height, const std::tstring& WindowName)
	: Core(hInstance, Width, Height, WindowName)
{
	std::locale::global(std::locale("Korean"));
}

bool Sample::Init()
{
	return true;
}

bool Sample::Frame()
{
	return true;
}

bool Sample::Render()
{

	return true;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR szCmdLine, int nCmdShow)
{
	try
	{
		Sample sd(hInstance, 800, 600, L"FirstWindow");

		if (!sd.Initialize())
			return 0;

		return sd.Run();
	}
	catch (DxException& e)
	{
		MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
		return 0;
	}
}