#include "stdafx.h"
#include "Sample.h"
#include "DirectInput.h"
#include "Converter.h"

using namespace std::chrono;

using namespace DirectX;

Sample::Sample(HINSTANCE hInstance, UINT Width, UINT Height, const std::tstring& WindowName)
	: Core(hInstance, Width, Height, WindowName)
{
	std::locale::global(std::locale("Korean"));
}

Sample::Sample(HINSTANCE hInstance, HWND hWnd, RECT rt)
	: Core(hInstance, hWnd, rt)
{
}

void Sample::setWireFrame()
{
	static bool isWire = false;
	isWire = !isWire;
	if (isWire)
	{
		mDxObj[DxType::SKINNED]->m_RasterizerState = E_RSS::Wireframe;
	}
	else
	{
		mDxObj[DxType::SKINNED]->m_RasterizerState = E_RSS::Default;
	}
}

void Sample::setBackColor(float color[4])
{
	mBackColor = XMFLOAT4(color);
}

void Sample::setSkyBox()
{
	misSkybox = !misSkybox;
}

bool Sample::Init()
{
	LightStorage * light = LightStorage::getLight();

	std::unique_ptr<LightProperty> l0 = std::make_unique<LightProperty>();

	XMStoreFloat3(&l0->light.Direction, XMVector3Normalize(-XMVectorSplatOne()));
	l0->light.Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
	l0->light.Strength = XMFLOAT3(0.9f, 0.9f, 0.9f);

	l0->Axis = DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f);
	l0->isRotate = true;
	l0->isClockwise = true;

	light->AddDirectional(l0);

	l0 = std::make_unique<LightProperty>();
	XMStoreFloat3(&l0->light.Direction, XMVector3Normalize(-XMVectorSet(1.0f, 1.0f, -1.0f, 0.0f)));
	l0->light.Strength = XMFLOAT3(0.65f, 0.65f, 0.65f);

	light->AddDirectional(l0);

	l0 = std::make_unique<LightProperty>();
	XMStoreFloat3(&l0->light.Direction, XMVector3Normalize(-XMVectorSet(-1.0f, 1.0f, -1.0f, 0.0f)));
	l0->light.Strength = XMFLOAT3(0.4f, 0.4f, 0.4f);

	light->AddDirectional(l0);

	//Converter con;
	//con.ConverttoSBI(L"sw.skn");

	grid.SetProperties(150.0f, 150.0f, 50, 50);
	grid.Create(m_pd3dDevice.Get(), L"..\\..\\data\\tile\\tile.dds", L"..\\..\\data\\tile\\tile_nmap.dds");

	steady_clock::time_point bef = steady_clock::now();
	mesh.LoadFile(L"death.sbi", L"..\\..\\data\\tex\\", m_pd3dDevice.Get());
	steady_clock::time_point aft = steady_clock::now();
	milliseconds u = duration_cast<milliseconds>(aft - bef);
	
	XMMATRIX T = XMMatrixTranslation(0.0f, 55.0f, 0.0f);
	XMMATRIX S = XMMatrixScaling(0.05f, 0.05f, 0.05f);
	mesh.SetWorld(S * T);

//	mesh.LoadFile(L"death.clb");
	//steady_clock::time_point aft0 = steady_clock::now();

	//milliseconds v = duration_cast<milliseconds>(aft0 - aft);

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

	mDxObj[DxType::SKINNED]->SetResource(m_pImmediateContext.Get());
	mesh.Render(m_pImmediateContext.Get());

	//mDxObj[DxType::NORMAL]->SetResource(m_pImmediateContext.Get());
	//mesh.Render(m_pImmediateContext.Get());
	return true;
}