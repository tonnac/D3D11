
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
	steady_clock::time_point bef = steady_clock::now();
	mesh.LoadFile(L"death.sbi", L"..\\..\\data\\tex\\", m_pd3dDevice.Get());
	steady_clock::time_point aft = steady_clock::now();
	seconds u = duration_cast<seconds>(aft - bef);

	XMMATRIX T = XMMatrixTranslation(0.0f, 55.0f, 0.0f);
	XMMATRIX S = XMMatrixScaling(0.05f, 0.05f, 0.05f);
	mesh.SetWorld(S * T);
	mesh.LoadFile(L"death.clb");
	steady_clock::time_point aft0 = steady_clock::now();

	seconds v = duration_cast<seconds>(aft0 - aft);

	return true;
}

bool Sample::Frame()
{
	mesh.Frame();
	return true;
}

bool Sample::Render()
{
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