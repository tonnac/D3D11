#include "Sample.h"

Sample::Sample()
{
}

bool Sample::Init()
{
	m_DirectWrite.Init();
	IDXGISwapChain * pSwapChain = getSwapChain();
	IDXGISurface* pSurface = nullptr;
	pSwapChain->GetBuffer(0, __uuidof(IDXGISurface), (LPVOID*)&pSurface);
	m_DirectWrite.Set(pSurface);
	pSurface->Release();
	return true;
}
bool Sample::Frame()
{
	return true;
}
bool Sample::Render()
{
	m_DirectWrite.Begin();

	D2D1_RECT_F pe = D2D1::RectF(0, 0, static_cast<FLOAT>(g_rtClient.right), static_cast<FLOAT>(g_rtClient.bottom));
	m_DirectWrite.DrawText(pe,L"HelloWorld",D2D1::ColorF::Tan);

	m_DirectWrite.End();
	return true;
}
bool Sample::Release()
{

	return true;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInst, LPWSTR szCmdLine, int nCmdShow)
{
	Sample wd;
	wd.InitWindow(hInstance, 800, 600, nCmdShow, L"FullScreen");
	wd.Run();
	return 0;
}