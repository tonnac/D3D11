
#include "Sample.h"
#include "DirectInput.h"

#include <chrono>

using namespace std::chrono;

using namespace DirectX;

Sample::Sample(HINSTANCE hInstance, UINT Width, UINT Height, const std::tstring& WindowName)
	: Core(hInstance, Width, Height, WindowName)
{
	std::locale::global(std::locale("Korean"));
}

bool Sample::Init()
{
	steady_clock::time_point bef = steady_clock::now();
	steady_clock::time_point aft[3];
	
	mesh.LoadFile(L"gggg2.ZXCS", m_pd3dDevice.Get());
	
	aft[0] = steady_clock::now();
	
	seconds p = duration_cast<seconds>(aft[0] - bef);

	return true;
}

bool Sample::Frame()
{
	mesh.Frame();
	S_RItem.UpdateObjectCBs(m_pImmediateContext.Get());
	return true;
}

bool Sample::Render()
{
	mesh.Render(m_pImmediateContext.Get());
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