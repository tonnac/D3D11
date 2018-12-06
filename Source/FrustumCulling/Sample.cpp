
#include "Sample.h"
#include "DirectInput.h"

using namespace DirectX;

Sample::Sample(HINSTANCE hInstance, UINT Width, UINT Height, const std::tstring& WindowName)
	: Core(hInstance, Width, Height, WindowName), m_Box(true)
{}

bool Sample::Init()
{
	d3dUtil::LoadPixelShaderFile(m_pd3dDevice.Get(), L"frustum1.hlsl", mPixelShader[0].GetAddressOf());
	d3dUtil::LoadPixelShaderFile(m_pd3dDevice.Get(), L"frustum2.hlsl", mPixelShader[1].GetAddressOf());


	m_Box.Create(m_pd3dDevice.Get(), L"shape.hlsl", L"../../data/misc/dice_unwrap.png");
	XMMATRIX minimapLook = XMMatrixLookAtLH(XMVECTOR({ 0.0f, 250.0f, -0.1f }), XMVectorZero(), XMVECTOR({ 0.0f, 1.0f, 0.0f }));
	m_Minimap.BuildMinimap(m_pd3dDevice.Get(), 1024, 1024, L"Minimap.hlsl", minimapLook);
	mFrustum.Initialize(m_pd3dDevice.Get(), L"frustum.hlsl", m_pMainCamera->m_matView, m_pMainCamera->m_matProj);

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
	m_Minimap.Begin(m_pImmediateContext.Get(), Colors::SkyBlue);
	{
		
		for (int i = 0; i < 100; ++i)
		{
			m_Box.SetMatrix(&mWorldarr[i]);
			if (mFrustum.Contains({ mWorldarr[i]._41, mWorldarr[i]._42, mWorldarr[i]._43 }) == true)
			{
				m_Box.Render(m_pImmediateContext.Get());
			}
			else
			{
				if (mFrustum.Contains(box[i]) == true)
				{
					m_Box.PreRender(m_pImmediateContext.Get());
					m_pImmediateContext->PSSetShader(mPixelShader[0].Get(), nullptr, 0);
					m_Box.PostRender(m_pImmediateContext.Get());
				}
				else
				{
					m_Box.PreRender(m_pImmediateContext.Get());
					m_pImmediateContext->PSSetShader(mPixelShader[1].Get(), nullptr, 0);
					m_Box.PostRender(m_pImmediateContext.Get());
				}
			}
			
		}
		mFrustum.Render(m_pImmediateContext.Get());
	}
	m_Minimap.End(m_pImmediateContext.Get(), &m_DxRT);

	int iCount = 0;
	for (int i = 0; i < 100; ++i)
	{
		if (mFrustum.Contains(box[i]) == false)
		{
			continue;
		}
		m_Box.SetMatrix(&mWorldarr[i]);
		m_Box.Render(m_pImmediateContext.Get());
		++iCount;
	}
	std::tstring text = L"InFrustum: " + std::to_tstring(iCount);
	m_Minimap.Render(m_pImmediateContext.Get(), 0, 300, 300, 300, text);	
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