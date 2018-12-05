
#include "Sample.h"
#include "DirectInput.h"

using namespace DirectX;

Sample::Sample(HINSTANCE hInstance, UINT Width, UINT Height, const std::tstring& WindowName)
	: Core(hInstance, Width, Height, WindowName), m_Box(true)
{}

bool Sample::Init()
{
	m_Box.Create(m_pd3dDevice.Get(), L"shape.hlsl", L"../../data/misc/dice_unwrap.png");
	XMMATRIX minimapLook = XMMatrixLookAtLH(XMVECTOR({ 0.0f, 150.0f, -0.1f }), XMVectorZero(), XMVECTOR({ 0.0f, 1.0f, 0.0f }));
	m_Minimap.BuildMinimap(m_pd3dDevice.Get(), 1024, 1024, L"Minimap.hlsl", minimapLook);
	mFrustum.Initialize(m_pd3dDevice.Get(), L"frustum.hlsl", m_pMainCamera->m_matView, m_pMainCamera->m_matProj);

	return true;
}

bool Sample::Frame()
{
	m_Box.Frame();
	mFrustum.UpdateFrustum(m_pMainCamera->m_matView, m_pMainCamera->m_matProj);
	m_Minimap.Frame();
	return true;
}

bool Sample::Render()
{
	m_Box.SetMatrix();
	m_Dir.SetMatrix();

	m_Minimap.Begin(m_pImmediateContext.Get(), Colors::SkyBlue);
	{
		m_Box.Render(m_pImmediateContext.Get());
		mFrustum.Render(m_pImmediateContext.Get());
	}
	m_Minimap.End(m_pImmediateContext.Get(), &m_DxRT);


	m_Box.Render(m_pImmediateContext.Get());
	m_pImmediateContext->OMSetBlendState(DxState::m_BSS[(int)E_BSS::No].Get(), 0, -1);
	m_Minimap.Render(m_pImmediateContext.Get(), {0 ,300, 300, 300});	
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