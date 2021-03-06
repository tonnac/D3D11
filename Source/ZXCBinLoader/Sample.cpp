
#include "Sample.h"
#include "DirectInput.h"
#include "ZXCBinLoader.h"

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
	mesh.LoadFile(L"S.BIN", L"..\\..\\data\\tex\\", m_pd3dDevice.Get());
	return true;
}

bool Sample::Frame()
{
	return true;
}

bool Sample::Render()
{
	mDxObj[DxType::DEFAULT]->SetResource(m_pImmediateContext.Get());
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