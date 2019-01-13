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
//	Converter con;
	//con.ConverttoSBI(L"Model\\shinbi\\shinbi.skn");

	grid.SetProperties(150.0f, 150.0f, 50, 50);
	grid.Create(m_pd3dDevice.Get(), L"..\\..\\data\\tile\\tile.dds", L"..\\..\\data\\tile\\tile_nmap.dds");

	steady_clock::time_point bef = steady_clock::now();
	mesh.LoadFile(L"Model\\shinbi\\shinbi.sbi", L"..\\..\\data\\tex\\shinbi\\", m_pd3dDevice.Get());

	XMMATRIX T = XMMatrixTranslation(0.0f, 50.0f, 0.0f);
	XMMATRIX S = XMMatrixScaling(0.05f, 0.05f, 0.05f);

//	XMMATRIX T = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
//	XMMATRIX S = XMMatrixScaling(15.0f, 15.0f, 15.0f);
	mesh.SetWorld(S);

	mesh.LoadFile(L"clip\\shinbi\\LevelStart.clb");
	mesh.LoadFile(L"clip\\shinbi\\AttackB.clb");
	mesh.LoadFile(L"clip\\shinbi\\AttackC.clb");
	mesh.LoadFile(L"clip\\shinbi\\AttackD.clb");


	//steady_clock::time_point aft0 = steady_clock::now();

	//milliseconds v = duration_cast<milliseconds>(aft0 - aft);
	steady_clock::time_point aft = steady_clock::now();
	seconds u = duration_cast<seconds>(aft - bef);
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

	//mDxObj[DxType::SKINNED]->SetResource(m_pImmediateContext.Get());
	//mesh.DebugRender(m_pImmediateContext.Get());

//	mDxObj[DxType::NORMAL]->SetResource(m_pImmediateContext.Get());
	//mesh.Render(m_pImmediateContext.Get());
	return true;
}