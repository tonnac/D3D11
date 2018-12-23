
#include "Sample.h"
#include "DirectInput.h"

using namespace DirectX;

Sample::Sample(HINSTANCE hInstance, UINT Width, UINT Height, const std::tstring& WindowName)
	: Core(hInstance, Width, Height, WindowName), m_Box(true)
{}

bool Sample::Init()
{
	m_Box.Create(m_pd3dDevice.Get(), L"box.hlsl", L"../../data/misc/dice_unwrap.png");

	XMVECTOR right = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR look = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	XMVECTOR vMax = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
	XMVECTOR vMin = XMVectorSet(-1.0f, -1.0f, -1.0f, 0.0f);
	for (int i = 0; i < 100; ++i)
	{
		XMFLOAT3 t = XMFLOAT3(MathHelper::RandF(-75.0f, 75.0f), MathHelper::RandF(-75.0f, 75.0f), MathHelper::RandF(-75.0f, 75.0f));
		box[i].mCenter = { t.x, t.y, t.z };

		XMMATRIX s = XMMatrixScaling(MathHelper::RandF(3.5f, 7.0f), MathHelper::RandF(3.5f, 7.0f), MathHelper::RandF(3.5f, 7.0f));
		XMMATRIX r = XMMatrixRotationRollPitchYaw(MathHelper::RandF(0.0f, XM_2PI), MathHelper::RandF(0.0f, XM_2PI), MathHelper::RandF(0.0f, XM_2PI));

		XMMATRIX sr = s * r;
		
		XMVECTOR p = XMLoadFloat3(&t);
		XMVECTOR axis[3];
		axis[0] = XMVector3TransformCoord(right, sr);
		axis[1] = XMVector3TransformCoord(up, sr);
		axis[2] = XMVector3TransformCoord(look, sr);
		XMStoreFloat3(&box[i].mAxis[0], XMVector3Normalize(axis[0]));
		XMStoreFloat3(&box[i].mAxis[1], XMVector3Normalize(axis[1]));
		XMStoreFloat3(&box[i].mAxis[2], XMVector3Normalize(axis[2]));

		sr.r[3].m128_f32[0] = t.x;
		sr.r[3].m128_f32[1] = t.y;
		sr.r[3].m128_f32[2] = t.z;
		sr.r[3].m128_f32[3] = 1.0f;

		XMVECTOR max = XMVector3Transform(vMax, sr);
		XMVECTOR min = XMVector3Transform(vMin, sr);
		XMVECTOR half = max - XMLoadFloat3(&box[i].mCenter);
		
		box[i].mExtent[0] = XMVectorGetX(XMVector3Dot(XMLoadFloat3(&box[i].mAxis[0]), half));
		box[i].mExtent[1] = XMVectorGetX(XMVector3Dot(XMLoadFloat3(&box[i].mAxis[1]), half));
		box[i].mExtent[2] = XMVectorGetX(XMVector3Dot(XMLoadFloat3(&box[i].mAxis[2]), half));

		XMStoreFloat4x4(&mWorldarr[i], sr);
	}

	D3D11_BUFFER_DESC bufDesc = CD3D11_BUFFER_DESC(sizeof(XMFLOAT4X4) * 100, D3D11_BIND_VERTEX_BUFFER);

	m_pd3dDevice->CreateBuffer(&bufDesc, nullptr, mInstanceBuffer.GetAddressOf());

	return true;
}

bool Sample::Frame()
{
	m_Box.Frame();
	for (int i = 0; i < 100; ++i)
	{
		XMMATRIX T = XMLoadFloat4x4(&mWorldarr[i]);
		XMStoreFloat4x4(&mTWorldarr[i], XMMatrixTranspose(T));
	}
	m_pImmediateContext->UpdateSubresource(mInstanceBuffer.Get(), 0, nullptr, mTWorldarr.data(), 0, 0);
	return true;
}

bool Sample::Render()
{
	int iCount = 0;
	UINT stride = sizeof(XMFLOAT4X4);
	UINT offset = 0;
	m_pImmediateContext->IASetVertexBuffers(1, 1, mInstanceBuffer.GetAddressOf(), &stride, &offset);
	m_Box.PreRender(m_pImmediateContext.Get());
	m_pImmediateContext->DrawIndexedInstanced(36, 100, 0, 0, 0);
	++iCount;
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