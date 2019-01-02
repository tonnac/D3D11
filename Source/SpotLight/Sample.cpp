
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
	LightStorage * light = LightStorage::getLight();

	Light l0;

	XMStoreFloat3(&l0.Direction, XMVector3Normalize(-XMVectorSplatOne()));
	l0.Strength = XMFLOAT3(0.9f, 0.9f, 0.9f);

	Light l1;

	XMStoreFloat3(&l1.Direction, XMVector3Normalize(-XMVectorSet(1.0f, 1.0f, -1.0f, 0.0f)));
	l1.Strength = XMFLOAT3(0.4f, 0.4f, 0.4f);

	Light l2;

	XMStoreFloat3(&l2.Direction, XMVector3Normalize(-XMVectorSet(-1.0f, 1.0f, -1.0f, 0.0f)));
	l2.Strength = XMFLOAT3(0.25f, 0.25f, 0.25f);

	light->AddDirectional(l0);
	light->AddDirectional(l1);
	light->AddDirectional(l2);
	
	grid.Create(m_pd3dDevice.Get(), L"..\\..\\data\\tile\\tile.dds", L"..\\..\\data\\tile\\tile_nmap.dds");
	spe.Create(m_pd3dDevice.Get());
	mesh.LoadFile(L"hhh.ZXCS", L"..\\..\\data\\tex\\", m_pd3dDevice.Get());
	return true;
}

bool Sample::Frame()
{
	mesh.Frame();
	return true;
}

bool Sample::Render()
{
	mDxObj[DxType::DEFAULT]->SetResource(m_pImmediateContext.Get());
	grid.Render(m_pImmediateContext.Get());
	mDxObj[DxType::NOTEX]->SetResource(m_pImmediateContext.Get());
	spe.Render(m_pImmediateContext.Get());
	mDxObj[DxType::SKINNED]->SetResource(m_pImmediateContext.Get());
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