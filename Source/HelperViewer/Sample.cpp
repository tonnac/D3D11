
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
	
	mesh.LoadFile(L"bbb.ZXCS", m_pd3dDevice.Get());

	aft[0] = steady_clock::now();
	
	seconds p = duration_cast<seconds>(aft[0] - bef);
	//meshes[0].LoadFile(L"bbb.ZXCS", m_pd3dDevice.Get());
	//aft[0] = steady_clock::now();
	//duration<double> af0 = aft[0] - bef;
	//meshes[1].LoadFile(L"ccc2.ZXCS", m_pd3dDevice.Get());
	//aft[1] = steady_clock::now();
	//duration<double> af1 = aft[1] - aft[0];
	//meshes[2].LoadFile(L"oo2.ZXCS", m_pd3dDevice.Get());
	//aft[2] = steady_clock::now();
	//duration<double> af2 = aft[2] - aft[1];

	return true;
}

bool Sample::Frame()
{
	//meshes[0].SetWorld(XMMatrixTranslation(-40.0f, 0.0f, 0.0f));
	//meshes[1].SetWorld(XMMatrixTranslation(0.0f, 0.0f, 0.0f));
	//meshes[2].SetWorld(XMMatrixTranslation(40.0f, 0.0f, 0.0f));
	//for (auto& x : meshes)
	//{
	//	x.Frame();
	//}
	mesh.Frame();
	S_RItem.UpdateObjectCBs(m_pImmediateContext.Get());
	return true;
}

bool Sample::Render()
{
	//for (auto&x : meshes)
	//{
	//	x.Render(m_pImmediateContext.Get());
	//}
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