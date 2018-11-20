
#include "Sample.h"
#include "DirectInput.h"

using namespace DirectX;

Sample::Sample(HINSTANCE hInstance, UINT Width, UINT Height, const std::tstring& WindowName)
	: Core(hInstance, Width, Height, WindowName)
{}

bool Sample::Init()
{
	m_Plane.Create(m_pd3dDevice.Get(), L"shape.hlsl", L"../../data/effect/Particle3.dds");
	m_Box.Create(m_pd3dDevice.Get(), L"shape.hlsl", L"../../data/effect/Particle3.dds");
	return true;
}

bool Sample::Frame()
{
	m_Box.Frame();
	return true;
}

bool Sample::Render()
{
	static float dll = 0;
	dll += m_Timer.DeltaTime();

	mWorld = MathHelper::Identity4x4();

	XMVECTOR Axis = { 8,2,4 };
	XMVECTOR N = XMVector3Normalize(Axis);

	float fSin, fCos;
	XMScalarSinCos(&fSin, &fCos, dll);

	N *= fSin;

	XMFLOAT3 nn;
	XMStoreFloat3(&nn, N);

	XMVECTOR quat1 = { nn.x, nn.y, nn.z, fCos };

	XMMATRIX Rot = XMMatrixRotationQuaternion(quat1);
	
	float Angle;
	XMVECTOR Axi1s;
	XMFLOAT3 ee;
	XMQuaternionToAxisAngle(&Axi1s, &Angle, quat1);
	XMVECTOR ANormal = XMVector3Normalize(Axi1s);
	float DAngle = XMConvertToDegrees(Angle);
	XMVECTOR fL = XMVector3Length(ANormal);
	XMStoreFloat3(&ee, ANormal);

	XMFLOAT4X4 bWorld;
	XMStoreFloat4x4(&bWorld, Rot);

	m_Box.SetMatrix(&bWorld, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	m_line.SetMatrix(&mWorld, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	m_Dir.SetMatrix(&mWorld, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);

	XMMATRIX Move = XMMatrixTranslation(5, 0, 0);
	
	XMStoreFloat4x4(&mWorld, Move);
	m_Plane.SetMatrix(&mWorld, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);

	m_Box.Render(m_pImmediateContext.Get());
	m_Plane.Render(m_pImmediateContext.Get());
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