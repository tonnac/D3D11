
#include "Sample.h"
#include "DirectInput.h"

using namespace DirectX;

Sample::Sample(HINSTANCE hInstance, UINT Width, UINT Height, const std::tstring& WindowName)
	: Core(hInstance, Width, Height, WindowName), m_Box(true)
{}

void Sample::FramePassCB()
{
	XMMATRIX View = XMLoadFloat4x4(&m_pMainCamera->m_matView);
	XMMATRIX Proj = XMLoadFloat4x4(&m_pMainCamera->m_matProj);

	XMMATRIX InvView = XMMatrixInverse(&XMMatrixDeterminant(View), View);
	XMMATRIX InvProj = XMMatrixInverse(&XMMatrixDeterminant(Proj), Proj);

	XMMATRIX ViewProj = View * Proj;
	XMMATRIX InvViewProj = XMMatrixInverse(&XMMatrixDeterminant(ViewProj), ViewProj);

	XMStoreFloat4x4(&mMainPassCB.View, XMMatrixTranspose(View));
	XMStoreFloat4x4(&mMainPassCB.Proj, XMMatrixTranspose(Proj));
	XMStoreFloat4x4(&mMainPassCB.InvView, XMMatrixTranspose(InvView));
	XMStoreFloat4x4(&mMainPassCB.InvProj, XMMatrixTranspose(InvProj));
	XMStoreFloat4x4(&mMainPassCB.ViewProj, XMMatrixTranspose(ViewProj));
	XMStoreFloat4x4(&mMainPassCB.InvViewProj, XMMatrixTranspose(InvViewProj));

	mMainPassCB.NearZ = 1.0f;
	mMainPassCB.FarZ = 1000.0f;
	mMainPassCB.TotalTime = m_Timer.DeltaTime();
	mMainPassCB.DeltaTime = m_Timer.TotalTime();

	m_pImmediateContext->UpdateSubresource(mPassCB.Get(), 0, nullptr, &mMainPassCB, 0, 0);
}

void Sample::FrameTexCB()
{
	XMMATRIX View = XMMatrixLookAtLH(XMVECTOR({ 0.0f, 10.0f, 0.1f }), XMVectorZero(), XMVECTOR({ 0.0f, 1.0f, 0.0f }));

	XMMATRIX Proj = XMLoadFloat4x4(&m_pMainCamera->m_matProj);

	XMMATRIX InvView = XMMatrixInverse(&XMMatrixDeterminant(View), View);
	XMMATRIX InvProj = XMMatrixInverse(&XMMatrixDeterminant(Proj), Proj);

	XMMATRIX ViewProj = View * Proj;
	XMMATRIX InvViewProj = XMMatrixInverse(&XMMatrixDeterminant(ViewProj), ViewProj);

	XMStoreFloat4x4(&mTexPassCB.View, XMMatrixTranspose(View));
	XMStoreFloat4x4(&mTexPassCB.Proj, XMMatrixTranspose(Proj));
	XMStoreFloat4x4(&mTexPassCB.InvView, XMMatrixTranspose(InvView));
	XMStoreFloat4x4(&mTexPassCB.InvProj, XMMatrixTranspose(InvProj));
	XMStoreFloat4x4(&mTexPassCB.ViewProj, XMMatrixTranspose(ViewProj));
	XMStoreFloat4x4(&mTexPassCB.InvViewProj, XMMatrixTranspose(InvViewProj));

	mTexPassCB.NearZ = 1.0f;
	mTexPassCB.FarZ = 1000.0f;
	mTexPassCB.TotalTime = m_Timer.DeltaTime();
	mTexPassCB.DeltaTime = m_Timer.TotalTime();

	m_pImmediateContext->UpdateSubresource(mTexCB.Get(), 0, nullptr, &mTexPassCB, 0, 0);
}

bool Sample::Init()
{
	d3dUtil::CreateConstantBuffer(m_pd3dDevice.Get(), 1, sizeof(PassConstants), mPassCB.GetAddressOf());
	d3dUtil::CreateConstantBuffer(m_pd3dDevice.Get(), 1, sizeof(PassConstants), mTexCB.GetAddressOf());

	m_Box.Create(m_pd3dDevice.Get(), L"shape.hlsl", L"../../data/misc/dice_unwrap.png");
	XMMATRIX minimapLook = XMMatrixLookAtLH(XMVECTOR({ 0.0f, 10.0f, 0.1f }), XMVectorZero(), XMVECTOR({ 0.0f, 1.0f, 0.0f }));
	m_Minimap.BuildMinimap(m_pd3dDevice.Get(), 1024, 1024, L"Minimap.hlsl", minimapLook);

	return true;
}

bool Sample::Frame()
{
	FramePassCB();
	FrameTexCB();
	m_Box.Frame();
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
	}
	m_Minimap.End(m_pImmediateContext.Get());



	m_pImmediateContext->RSSetViewports(1, &m_Viewport);
	m_pImmediateContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
	m_pImmediateContext->VSSetConstantBuffers(0, 1, mPassCB.GetAddressOf());
	m_Box.Render(m_pImmediateContext.Get());


	m_Minimap.Render(m_pImmediateContext.Get(), {0 ,400, 200, 200});
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