#include "DirectWrite.h"

bool DirectWrite::Set(IDXGISwapChain* pSwapChain)
{
	CreateINDP_Device();
	CreateDP_Device(pSwapChain);
	return true;
}
bool DirectWrite::Init()
{
	return true;
}
bool DirectWrite::Begin()
{
	if (m_pRenderTarget != nullptr)
	{
		m_pRenderTarget->BeginDraw();
	}
	return true;
}
bool DirectWrite::End()
{
	if (m_pRenderTarget != nullptr)
	{
		m_pRenderTarget->EndDraw();
	}
	return true;
}
bool DirectWrite::Release()
{
	return true;
}
void DirectWrite::DrawText(const D2D1_RECT_F& rt, const std::tstring& Text, D2D1::ColorF Color)
{
	m_pColorBrush->SetColor(Color);
	m_pRenderTarget->DrawText(Text.c_str(), CASTING(UINT, Text.length()), m_pTextFormat, rt, m_pColorBrush);
}
void DirectWrite::OnResize(IDXGISwapChain * pSwapChain)
{
	DiscardDP_Device();
	CreateDP_Device(pSwapChain);
}
void DirectWrite::CreateINDP_Device()
{
	ThrowifFailed(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory));
	m_pD2DFactory->GetDesktopDpi(&m_fdpiX, &m_fdpiY);
	ThrowifFailed(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), RE_CASTING(IUnknown**, &m_pWriteFactory)));
	ThrowifFailed(m_pWriteFactory->CreateTextFormat(L"°íµñ", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20, L"en-us", &m_pTextFormat));
}
void DirectWrite::CreateDP_Device(IDXGISwapChain* pSwapChain)
{
	IDXGISurface* pSurface = nullptr;
	ThrowifFailed(pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pSurface)));

	D2D1_RENDER_TARGET_PROPERTIES Property;
	Property.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
	Property.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
	Property.pixelFormat.format = DXGI_FORMAT_R8G8B8A8_UNORM;
	Property.dpiX = m_fdpiX;
	Property.dpiY = m_fdpiY;
	Property.usage = D2D1_RENDER_TARGET_USAGE_NONE;
	Property.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;

	ThrowifFailed(m_pD2DFactory->CreateDxgiSurfaceRenderTarget(pSurface, &Property, &m_pRenderTarget));
	ThrowifFailed(m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::AliceBlue), &m_pColorBrush));
	RELEASE(pSurface);
}
void DirectWrite::DiscardINDP_Device()
{
	DiscardDP_Device();
	RELEASE(m_pD2DFactory);
	RELEASE(m_pWriteFactory);
	RELEASE(m_pTextFormat);
}
void DirectWrite::DiscardDP_Device()
{
	RELEASE(m_pColorBrush);
	RELEASE(m_pRenderTarget);
}