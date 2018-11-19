
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

	m_Box.SetMatrix(&mWorld, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	m_line.SetMatrix(&mWorld, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	m_Dir.SetMatrix(&mWorld, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	XMMATRIX Move = XMMatrixTranslation(5, 0, 0);
	XMStoreFloat4x4(&mWorld, Move);
	m_Plane.SetMatrix(&mWorld, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);

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