#include "Core.h"

Core::Core()
{}

bool Core::GameInit()
{
	PreInit();

	if (FAILED(CreateDevice())) return false;
	if (FAILED(CreateFactory())) return false;
	if (FAILED(CreateSwapChain())) return false;
	if (FAILED(SetRenderTargetView())) return false;
	SetViewPort();

	IDXGIFactory * pFactory = getDXGIFactory();

	//Alt+Enter 와 윈도우 메시지 막음
	UINT Flags = DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER;
	pFactory->MakeWindowAssociation(g_hWnd, Flags);
	m_Timer.Init();
	S_Write.Init(getSwapChainPtr());
	Init();

	S_Write.AddText(L"FPS", L"qqq", D2D1::RectF(0, 0, 800, 600), D2D1::ColorF::SteelBlue, L"Gabriola", 20);
	S_Write.SetAlignment(L"FPS", DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	S_Write.AddText(L"PPE", L"Make_Pair", D2D1::RectF(0, 0, 800, 600), D2D1::ColorF::DarkCyan, L"고딕", 23);
	S_Write.AddLayout(L"PPE", { 240.0f,300.0f });
	S_Write.SetFont(L"PPE",L"Gabriola");
	S_Write.SetWeight(L"PPE",true);
	S_Write.SetFontSize(L"PPE",40);
	S_Write.SetStyle(L"PPE",true);
//	S_Write.SetUnderline(L"PPE",true);
	return true;
}
bool Core::GameRun()
{
	GameFrame();
	GameRender();
	return true;
}
bool Core::GameRelease()
{
	CleanupDevice();
	S_Write.Release();
	m_Timer.Release();
	Release();
	return true;
}

bool Core::Init()
{
	return true;
}
bool Core::Frame()
{
	return true;
}
bool Core::Render()
{
	return true;
}
bool Core::Release()
{
	return true;
}
void Core::AddText(const TCHAR* Key, const std::tstring& Text, const D2D1_RECT_F& rt, const D2D1::ColorF& Color, const TCHAR* FontName,
	const FLOAT& fontSize, const TCHAR* localeName, const DWRITE_FONT_WEIGHT& FontWeight, const DWRITE_FONT_STYLE& FontStyle, const DWRITE_FONT_STRETCH& FontStretch)
{
	S_Write.AddText(Key, Text, rt, Color, FontName, fontSize, localeName, FontWeight, FontStyle, FontStretch);
}
bool Core::GameFrame()
{
	m_Timer.Frame();
	S_Write.ChangeText(L"FPS", m_Timer.getTimeInfo());
	Frame();
	return true;
}
bool Core::GameRender()
{
	if (PreRender() == false) return false;
	Render();
	m_Timer.Render();
	S_Write.Render();
	if (PostRender() == false) return false;
	return true;
}
bool Core::PreInit()
{
	return true;
}
bool Core::PreRender()
{
	ID3D11DeviceContext* pContext = getContext();
	ID3D11RenderTargetView* pRenderTargetView = getRenderTargetView();
	
	float Color[] = { 0.8f,1.0f,0.8f,1.0f };
	pContext->ClearRenderTargetView(pRenderTargetView, Color);
	S_Write.Begin();
	return true;
}
bool Core::PostRender()
{
	S_Write.End();
	IDXGISwapChain* pSwapChain = getSwapChain();
	pSwapChain->Present(0, 0);
	return true;
}