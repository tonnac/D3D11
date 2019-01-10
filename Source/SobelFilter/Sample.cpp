
#include "Sample.h"
#include "DirectInput.h"
#include "LightStorage.h"

#include <chrono>

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
	std::unique_ptr<LightProperty> l0 = std::make_unique<LightProperty>();
	l0->Type = LightType::Directional;
	l0->light.Direction = XMFLOAT3(-1.0f, -1.0f, -1.0f);
	l0->light.Strength = XMFLOAT3(1.0f, 1.0f, 1.0f);

	l0->DirRot.isRotate = true;
	l0->DirRot.Axis = XMFLOAT3(0.0f, 1.0f, 0.0f);
	l0->DirRot.RotSpeed = 1.0f;

	l->AddLight(l0);

	mSobelFilter = std::make_unique<SobelFilter>(m_pd3dDevice.Get(), g_ClientWidth, g_ClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, L"shaders\\SobelFilter.hlsl");

	mesh.LoadFile(L"sphere0.skn", L"..\\..\\data\\tile\\", m_pd3dDevice.Get());
	//mesh.LoadFile(L"sylbanas.sbi", L"..\\..\\data\\tex\\sylbanas\\", m_pd3dDevice.Get());

	XMMATRIX S = XMMatrixScaling(3.0f, 3.0f, 3.0f);
	mesh.SetWorld(S);

	mOffRT.Initialize(m_pd3dDevice.Get(), (float)g_ClientWidth, (float)g_ClientHeight);

	return true;
}

bool Sample::Frame()
{
	mesh.Frame();

	if (S_Input.getKeyState(DIK_G) == Input::KEYSTATE::KEY_PUSH)
	{
		ID3D11Texture2D* tex = mSobelFilter->Texture();
	//	ID3D11Texture2D* tex = mOffRT.Texture();
		D3DX11SaveTextureToFile(m_pImmediateContext.Get(), tex, D3DX11_IFF_DDS, L"texFile.dds");
	}

	return true;
}

bool Sample::Render()
{
	mOffRT.Render(m_pImmediateContext.Get(), &mesh, mDxObj[DxType::SKINNED].get());
	ID3D11ShaderResourceView** srv = mOffRT.GetSRV();
	mSobelFilter->Execute(m_pImmediateContext.Get(), srv);
	ID3D11ShaderResourceView** sobelView = mSobelFilter->ShaderResrouceView();

	m_pImmediateContext->OMSetRenderTargets(1, m_DxRT.RenderTargetView(), m_DxRT.DepthStencilView());
	m_pImmediateContext->RSSetViewports(1, &m_DxRT.Viewport());

	mDxObj[DxType::COMPOSITE]->SetResource(m_pImmediateContext.Get());
	m_pImmediateContext->PSSetShaderResources(0, 1, srv);
	m_pImmediateContext->PSSetShaderResources(1, 1, sobelView);
	m_pImmediateContext->DrawInstanced(6, 1, 0, 0);

	//mDxObj[DxType::SKINNED]->SetResource(m_pImmediateContext.Get());
	//mesh.Render(m_pImmediateContext.Get());

	

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