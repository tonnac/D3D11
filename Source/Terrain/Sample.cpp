#include "Sample.h"

bool Sample::Init()
{
	m_Background.SetPos(220.0f, 0.0f, 896.0f, 240.0f);
	m_Background.InitSet(m_pDevice, L"DD", L"../../momodora/data/map/Map1.png", L"VertexShader.txt");
	m_Terrain.SetPos({ 0.0f, 192.0f, 960.0f, 240.0f });
	m_Terrain.InitSet(m_pDevice, L"DD1", L"VertexShader.txt", "VS", "TerrainPS");

	return true;
}
bool Sample::Frame()
{
	D3DXMATRIX pe;
	D3DXMatrixIdentity(&pe);
	pe._11 = 2.0f / g_rtClient.right;
	pe._31 = -1.0f;
	pe._22 = -2.0f / g_rtClient.bottom;
	pe._32 = 1.0f;
	D3DXVECTOR3 qwe = { 480.0f,360.0f, 1.0f};
	D3DXVECTOR3 out = { 0.0f,0.0f, 0.0f};
	D3DXVec3TransformNormal(&out, &qwe, &pe);
	m_Background.Frame();
	m_Terrain.Frame();
	return true;
}
bool Sample::Render()
{
//	m_Background.Render(m_pContext);
	m_Terrain.Render(m_pContext);
	return true;
}
bool Sample::Release()
{
	m_Background.Release();
	m_Terrain.Release();
	return true;
}
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInst, LPWSTR szCmdLine, int nCmdShow)
{
	try
	{
		Sample sd;
		sd.Set(hInstance, 960, 720, L"Sample");
		return sd.Run();
	}
	catch (DxException &e)
	{
		MessageBox(nullptr, e.ToString().c_str(), L"FAILED", MB_OK);
		return 0;
	}
}
