
#include "Sample.h"

using namespace DirectX;

Sample::Sample(HINSTANCE hInstance, UINT Width, UINT Height, const std::tstring& WindowName)
	: Core(hInstance, Width, Height, WindowName)
{
	mWorld = MathHelper::Identity4x4();
	mView = MathHelper::Identity4x4();
	mProj = MathHelper::Identity4x4();
}

bool Sample::Init()
{
	m_Box.Create(m_pd3dDevice.Get(), L"shape.hlsl", L"../../data/map/Dirt_Diff.dds");
	
	XMMATRIX view = XMMatrixLookAtLH(XMVECTOR({ 3.0f,3.0f,-5.0f }), XMVECTOR({ 0.0f,0.0f, 0.0f }), XMVECTOR({ 0.0f,1.0f,0.0f }));
	XMMATRIX proj = XMMatrixPerspectiveFovLH(MathHelper::PI * 0.25f, AspectRatio(), 1.0f, 100.0f);

	XMStoreFloat4x4(&mView, view);
	XMStoreFloat4x4(&mProj, proj);

	m_Box.SetMatrix(&mWorld, &mView, &mProj);

	return true;
}

bool Sample::Frame()
{
	m_Box.Frame();
	return true;
}

bool Sample::Render()
{
	m_Box.Render(m_pImmediateContext.Get());
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