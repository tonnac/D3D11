
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
	mesh0->LoadFile(L"Object\\turret.sbi", L"tex\\turret\\");	
	mesh0->LoadFile(L"Object\\Animations\\tur.clb");
	XMMATRIX T = XMMatrixTranslation(130, 0, 0);
	mesh0->SetWorld(T);

	mesh1 = std::make_unique<Mesh<>>(m_pd3dDevice.Get());
	mesh1->LoadFile(L"Object\\multicamera.zbi", L"tex\\multicamera\\");
	T = XMMatrixTranslation(0, 10, 0);
	mesh1->SetWorld(T);

	mesh2 = std::make_unique<Mesh<>>(m_pd3dDevice.Get());
	mesh2->LoadFile(L"Object\\ship.zbi", L"tex\\ship\\");
	T = XMMatrixTranslation(-130, 0, -250);
	mesh2->SetWorld(T);

	return true;
}

bool Sample::Frame()
{
	mesh0->Frame();
	mesh1->Frame();
	mesh2->Frame();
	return true;
}

bool Sample::Render()
{
	mDxObj[DxType::SKINNED]->SetResource(m_pImmediateContext.Get());
	mesh0->Render(m_pImmediateContext.Get());
	mDxObj[DxType::DEFAULT]->SetResource(m_pImmediateContext.Get());
	mesh1->Render(m_pImmediateContext.Get());
	mesh2->Render(m_pImmediateContext.Get());
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