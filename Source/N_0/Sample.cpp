
#include "Sample.h"
#include "DirectInput.h"
#include "LightStorage.h"
#include "DirectWrite.h"
#include "Converter.h"

#include <chrono>

using namespace std::chrono;

using namespace DirectX;
using namespace DirectX::Colors;


Sample::Sample(HINSTANCE hInstance, UINT Width, UINT Height, const std::tstring& WindowName)
	: Core(hInstance, Width, Height, WindowName)
{
	std::locale::global(std::locale("Korean"));
}

bool Sample::Init()
{
	mSobelfilter = std::make_unique<SobelFilter>(m_pd3dDevice.Get(), g_ClientWidth, g_ClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, L"shaders\\SobelFilter.hlsl");
	mDxRT = std::make_unique<DxRT>(m_pd3dDevice.Get(), g_ClientWidth, g_ClientHeight);

	//Converter con;
	//con.ConverttoSBI(L"Character\\shinbi.skn");

	std::shared_ptr<LightProperty>light = std::make_shared<LightProperty>();

	light->Type = LightType::Directional;
	light->light.Direction = XMFLOAT3(-1.0f, -1.0f, -1.0f);
	light->light.Strength = XMFLOAT3(0.95f, 0.95f, 0.95f);
	light->DirRot.isRotate = true;
	light->DirRot.Axis = XMFLOAT3(0, 1, 0);
	light->DirRot.isClockwise = true;
	light->DirRot.RotSpeed = 1.0f;

	LightStorage::getLight()->AddLight(light);

	light = std::make_shared<LightProperty>();

	light->Type = LightType::Directional;
	light->light.Direction = XMFLOAT3(-1.0f, -1.0f, 1.0f);
	light->light.Strength = XMFLOAT3(0.65f, 0.65f, 0.65f);
	light->DirRot.isRotate = true;
	light->DirRot.Axis = XMFLOAT3(0, 1, 0);
	light->DirRot.isClockwise = true;
	light->DirRot.RotSpeed = 1.0f;

	LightStorage::getLight()->AddLight(light);

	light = std::make_shared<LightProperty>();

	light->Type = LightType::Directional;
	light->light.Direction = XMFLOAT3(1.0f, -1.0f, 1.0f);
	light->light.Strength = XMFLOAT3(0.25f, 0.25f, 0.25f);
	light->DirRot.isRotate = true;
	light->DirRot.Axis = XMFLOAT3(0, 1, 0);
	light->DirRot.isClockwise = true;
	light->DirRot.RotSpeed = 1.0f;

	LightStorage::getLight()->AddLight(light);

	mesh0 = std::make_unique<SkinnedMesh>(m_pd3dDevice.Get());
	mesh0->LoadFile(L"Character\\Jaina.sbi", L"..\\..\\data\\tex\\Jaina\\");
	
	mesh0->LoadFile(L"Character\\Animation\\Jaina\\SpellCastDirected.clb");
	mesh0->LoadFile(L"Character\\Animation\\Jaina\\ChannelCastDirected.clb");
	mesh0->LoadFile(L"Character\\Animation\\Jaina\\SpellCastOmni.clb");
	mesh0->LoadFile(L"Character\\Animation\\Jaina\\ChannelCastOmni.clb");
	mesh0->LoadFile(L"Character\\Animation\\Jaina\\EmoteCheer.clb");

	XMMATRIX R = XMMatrixRotationY(XM_PIDIV2);

	mesh0->SetWorld(R);

	mesh1 = std::make_unique<SkinnedMesh>(m_pd3dDevice.Get());
	mesh1->LoadFile(L"Character\\shinbi.sbi", L"..\\..\\data\\tex\\shinbi\\");

	mesh1->LoadFile(L"Character\\Animation\\Shinbi\\LevelStart.clb");
	mesh1->LoadFile(L"Character\\Animation\\Shinbi\\Recall.clb");
	mesh1->LoadFile(L"Character\\Animation\\Shinbi\\AttackB.clb");
	mesh1->LoadFile(L"Character\\Animation\\Shinbi\\AttackC.clb");
	mesh1->LoadFile(L"Character\\Animation\\Shinbi\\AttackD.clb");

	XMMATRIX T = XMMatrixTranslation(30, 0, 0);

	mesh1->SetWorld(T);

	grid.SetProperties(150.0f, 150.0f, 50, 50);
	grid.Create(m_pd3dDevice.Get(), L"..\\..\\data\\tile\\tile.dds", L"..\\..\\data\\tile\\tile_nmap.dds");

	return true;
}

bool Sample::Frame()
{
	if (S_Input.getKeyState(DIK_1) == Input::KEYSTATE::KEY_PUSH)
	{
		misSobel = !misSobel;
	}

	mesh0->Frame();
	mesh1->Frame();
	return true;
}

bool Sample::Render()
{
	S_Write.DrawText({ 0, 20, 800, 600 }, L"1번키로 소벨필터 활성화", Colors::Black);

	mDxObj[DxType::SKINNED]->SetResource(m_pImmediateContext.Get());
	m_pImmediateContext->OMSetBlendState(DxState::m_BSS[(int)E_BSS::No].Get(), 0, -1);
	mesh0->Render(m_pImmediateContext.Get());
	mDxObj[DxType::SKINBUMP]->SetResource(m_pImmediateContext.Get());
	mesh1->Render(m_pImmediateContext.Get());
	mDxObj[DxType::BUMP]->SetResource(m_pImmediateContext.Get());
	grid.Render(m_pImmediateContext.Get());

	if (misSobel)
	{
		mDxRT->Begin(m_pImmediateContext.Get(), Colors::Black);
		{
			mDxObj[DxType::SKY]->SetResource(m_pImmediateContext.Get());
			mSkybox.Render(m_pImmediateContext.Get());
			mDxObj[DxType::SKINNED]->SetResource(m_pImmediateContext.Get());
			mesh0->Render(m_pImmediateContext.Get());
			mDxObj[DxType::SKINBUMP]->SetResource(m_pImmediateContext.Get());
			mesh1->Render(m_pImmediateContext.Get());
			mDxObj[DxType::BUMP]->SetResource(m_pImmediateContext.Get());
			grid.Render(m_pImmediateContext.Get());
		}
		mDxRT->End(m_pImmediateContext.Get());

		ID3D11ShaderResourceView** srv = mDxRT->GetSRV();

		mSobelfilter->Execute(m_pImmediateContext.Get(), srv);

		ID3D11ShaderResourceView** sobelView = mSobelfilter->ShaderResrouceView();

		m_pImmediateContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
		m_pImmediateContext->RSSetViewports(1, &m_Viewport);

		mDxObj[DxType::COMPOSITE]->SetResource(m_pImmediateContext.Get());
		m_pImmediateContext->PSSetShaderResources(0, 1, srv);
		m_pImmediateContext->PSSetShaderResources(1, 1, sobelView);
		m_pImmediateContext->DrawInstanced(6, 1, 0, 0);
	}

	return true;
}

void Sample::OnResize()
{
	Core::OnResize();
	if(mDxRT != nullptr)
		mDxRT->OnResize(g_ClientWidth, g_ClientHeight);
	if(mSobelfilter != nullptr)
		mSobelfilter->OnResize(g_ClientWidth, g_ClientHeight);
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