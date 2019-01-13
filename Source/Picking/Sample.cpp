
#include "Sample.h"
#include "DirectInput.h"
#include "LightStorage.h"
#include "Converter.h"

using namespace std::chrono;

using namespace DirectX;

struct Data
{
	XMFLOAT3 v1;
	XMFLOAT2 v2;
};

Sample::Sample(HINSTANCE hInstance, UINT Width, UINT Height, const std::tstring& WindowName)
	: Core(hInstance, Width, Height, WindowName)
{
	std::locale::global(std::locale("Korean"));
}

bool Sample::Init()
{
	auto* l = LightStorage::getLight();
	std::shared_ptr<LightProperty> l0 = std::make_unique<LightProperty>();
	l0->Type = LightType::Directional;
	l0->light.Direction = XMFLOAT3(-1.0f, -1.0f, -1.0f);
	l0->light.Strength = XMFLOAT3(1.0f, 1.0f, 1.0f);

	l0->DirRot.isRotate = true;
	l0->DirRot.Axis = XMFLOAT3(0.0f, 1.0f, 0.0f);
	l0->DirRot.RotSpeed = 1.0f;

	l->AddLight(l0);

	mSobelFilter = std::make_unique<Computeshader>(m_pd3dDevice.Get(), g_ClientWidth, g_ClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, L"shaders\\StencilCompute.hlsl");
	mOffRT = std::make_unique<DxRT>(m_pd3dDevice.Get(), g_ClientWidth, g_ClientHeight);

	mesh.LoadFile(L"shinbi.sbi", L"..\\..\\data\\tex\\shinbi\\", m_pd3dDevice.Get());
	XMMATRIX S = XMMatrixScaling(0.05f, 0.05f, 0.05f);

	mesh.SetWorld(S);

	return true;
}

bool Sample::Frame()
{
	mesh.Frame();

	if (S_Input.getKeyState(DIK_G) == Input::KEYSTATE::KEY_PUSH)
	{
	//	ID3D11Texture2D* tex = mSobelFilter->Texture();
		ID3D11Texture2D* tex = mOffRT->Texture();
		D3DX11SaveTextureToFile(m_pImmediateContext.Get(), tex, D3DX11_IFF_DDS, L"texFile.dds");
	}

	return true;
}

bool Sample::Render()
{
	if (isPicking)
	{
		mOffRT->Render(m_pImmediateContext.Get(), &mesh, mDxObj[DxType::SKINNED].get());
		ID3D11ShaderResourceView** srv = mOffRT->GetDSSrv();
		ID3D11ShaderResourceView** srv0 = mOffRT->GetSRV();
		mSobelFilter->Execute(m_pImmediateContext.Get(), srv);
		ID3D11ShaderResourceView** sobelView = mSobelFilter->ShaderResrouceView();

		m_pImmediateContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
		m_pImmediateContext->RSSetViewports(1, &m_Viewport);

		mDxObj[DxType::COMPOSITE]->SetResource(m_pImmediateContext.Get());
		m_pImmediateContext->PSSetShaderResources(0, 1, srv0);
		m_pImmediateContext->PSSetShaderResources(1, 1, sobelView);
		m_pImmediateContext->DrawInstanced(6, 1, 0, 0);
	}
	else
	{
		mDxObj[DxType::SKINNED]->SetResource(m_pImmediateContext.Get());
		mesh.Render(m_pImmediateContext.Get());
	}


	return true;
}

void Sample::Pick(int sx, int sy)
{
	XMFLOAT4X4 P = m_pMainCamera->m_matProj;

	float vx = (2.0f * sx / g_ClientWidth - 1.0f) / P(0, 0);
	float vy = (-2.0f * sy / g_ClientHeight + 1.0f) / P(1, 1);

	XMVECTOR rayOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR rayDir = XMVectorSet(vx, vy, 1.0f, 0.0f);

	XMMATRIX V = XMLoadFloat4x4(&m_pMainCamera->m_matView);
	XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(V), V);

	float tmin = 0.0f;
	isPicking = mesh.Intersects(rayOrigin, rayDir, invView, tmin);
}

void Sample::OnResize()
{
	Core::OnResize();
	if(mOffRT != nullptr)
		mOffRT->OnResize(g_ClientWidth, g_ClientHeight);
	if(mSobelFilter != nullptr)
		mSobelFilter->OnResize(g_ClientWidth, g_ClientHeight);
}

void Sample::OnMouseDblClk(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		Pick(x, y);
	}
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