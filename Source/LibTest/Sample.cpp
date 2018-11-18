
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
	m_Plane.Create(m_pd3dDevice.Get(), L"shape.hlsl", L"../../data/effect/Particle3.dds");
	m_Box.Create(m_pd3dDevice.Get(), L"shape.hlsl", L"../../data/effect/Particle3.dds");
	m_line.Create(m_pd3dDevice.Get(), L"shape.hlsl");
	
	XMMATRIX view = XMMatrixLookAtLH(XMVECTOR({ 3.0f,8.0f,-10.0f }), XMVECTOR({ 0.0f,0.0f, 0.0f }), XMVECTOR({ 0.0f,1.0f,0.0f }));
	XMMATRIX proj = XMMatrixPerspectiveFovLH(MathHelper::PI * 0.25f, AspectRatio(), 1.0f, 100.0f);

	XMStoreFloat4x4(&mView, view);
	XMStoreFloat4x4(&mProj, proj);

	m_Box.SetMatrix(&mWorld, &mView, &mProj);
	m_line.SetMatrix(&mWorld, &mView, &mProj);
	m_Dir.SetMatrix(&mWorld, &mView, &mProj);
	XMMATRIX Move = XMMatrixTranslation(5, 0, 0);
	XMStoreFloat4x4(&mWorld, Move);
	m_Plane.SetMatrix(&mWorld, &mView, &mProj);

	return true;
}

bool Sample::Frame()
{
	m_Box.Frame();
	return true;
}

bool Sample::Render()
{
	XMVECTOR pp = XMLoadFloat3(&m_Plane.m_vLook);
	pp *= 100;
	XMMATRIX move = XMMatrixTranslation(m_Plane.m_vPosition.x, m_Plane.m_vPosition.y, m_Plane.m_vPosition.z);
	XMVECTOR eee = XMVector3TransformCoord(pp, move);
	XMFLOAT3 Look;
	XMStoreFloat3(&Look, eee);

	m_Box.Render(m_pImmediateContext.Get());
	m_Plane.Render(m_pImmediateContext.Get());
	m_line.Draw(m_pImmediateContext.Get(), m_Plane.m_vPosition, Look, XMFLOAT4(Colors::Aqua));
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