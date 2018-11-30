
#include "Sample.h"
#include "DirectInput.h"

using namespace DirectX;

Sample::Sample(HINSTANCE hInstance, UINT Width, UINT Height, const std::tstring& WindowName)
	: Core(hInstance, Width, Height, WindowName)
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
}

bool Sample::Init()
{
	d3dUtil::CreateConstantBuffer(m_pd3dDevice.Get(), 1, sizeof(PassConstants), mPassCB.GetAddressOf());
	m_Plane.Create(m_pd3dDevice.Get(), L"shape.hlsl", L"../../data/effect/Particle3.dds");
	m_Box.Create(m_pd3dDevice.Get(), L"shape.hlsl", L"../../data/effect/Particle3.dds");
	return true;
}

bool Sample::Frame()
{
	FramePassCB();
	m_Box.Frame();
	return true;
}

bool Sample::Render()
{
	static float dll = 0;
	dll += m_Timer.DeltaTime();

	m_pImmediateContext->UpdateSubresource(mPassCB.Get(), 0, nullptr, &mMainPassCB, 0, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, mPassCB.GetAddressOf());

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

	m_Box.SetMatrix(&bWorld);
	m_line.SetMatrix();
	m_Dir.SetMatrix();

	XMMATRIX Move = XMMatrixTranslation(5, 0, 0);
	
	XMFLOAT4X4 Wd;
	XMStoreFloat4x4(&Wd, Move);
	m_Plane.SetMatrix(&Wd);

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