#include "Sample.h"

bool Sample::Init()
{
	m_object.InitSet(m_pDevice, L"Kaho", L"../../data/character/kaho.png", L"vertexshader.txt");
	CreateRasterizer();
	return true;
}
bool Sample::Frame()
{
	if (S_Input.getKeyState(DIK_A) == Input::KEYSTATE::KEY_HOLD)
	{
		m_pContext->RSSetState(m_pWireFrame);
	}
	else
		m_pContext->RSSetState(m_pRasterizer);
	m_object.Frame();
	return true;
}
bool Sample::Render()
{
	m_object.Render(m_pContext);
	return true;
}
bool Sample::Release()
{
	m_object.Release();
	return true;
}
void Sample::CreateRasterizer()
{
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_NONE;
	desc.DepthClipEnable = TRUE;
	ThrowifFailed(m_pDevice->CreateRasterizerState(&desc, &m_pRasterizer));
	desc.FillMode = D3D11_FILL_WIREFRAME;
	ThrowifFailed(m_pDevice->CreateRasterizerState(&desc, &m_pWireFrame));
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInst, LPWSTR szCmdLine, int nCmdShow)
{
	try
	{
		Sample sd;
		sd.Set(hInstance, 800, 600, L"Sample");
		return sd.Run();
	}
	catch (DxException &e)
	{
		MessageBox(nullptr, e.ToString().c_str(), L"FAILED", MB_OK);
		return 0;
	}
}
