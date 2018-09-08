#include "DirectWrite.h"

DirectWrite::DirectWrite() : m_pD2DFactory(nullptr), m_pRenderTarget(nullptr), m_pDWriteFactory(nullptr), m_pTextFormat(nullptr), m_pTextLayout(nullptr)
{}

bool DirectWrite::Set(IDXGISurface* pSurface)
{
	CreateDeviceIndependentResources();
	CreateDeviceResources(pSurface);
	return true;
}
bool DirectWrite::Init()
{
	return true;
}
bool DirectWrite::Release()
{
	DiscardDeviceResources();
	DiscardDeviceIndependentResources();
	return true;
}
bool DirectWrite::Begin()
{
	if (m_pRenderTarget)
	{
		m_pRenderTarget->BeginDraw();
		m_pRenderTarget->SetTransform(D2D1::IdentityMatrix());
	}
	return true;
}
bool DirectWrite::End()
{
	if (m_pRenderTarget && FAILED(m_pRenderTarget->EndDraw()))
	{
		return false;
	}
	return true;
}
HRESULT DirectWrite::DrawText(const D2D1_RECT_F& rt, const TCHAR* pText, D2D1::ColorF Color)
{
	if (m_pRenderTarget && m_pColorBrush)
	{
		m_pColorBrush->SetColor(Color);
		m_pRenderTarget->DrawText(pText, static_cast<UINT32>(_tcslen(pText)), m_pTextFormat,
			rt, m_pColorBrush);
	}
	return S_OK;
}
HRESULT	DirectWrite::CreateDeviceIndependentResources()
{
	// D2DFactory Create
	HRESULT hr;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
	m_pD2DFactory->GetDesktopDpi(&m_fDpiX, &m_fDpiY);

	// WriteFactroy Create
	if (SUCCEEDED(hr))
	{
		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(m_pDWriteFactory), reinterpret_cast<IUnknown**>(&m_pDWriteFactory));
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pDWriteFactory->CreateTextFormat(
			L"°íµñ", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
			20, L"ko-KR", &m_pTextFormat);
	}
	return hr;
}
void DirectWrite::DiscardDeviceIndependentResources()
{
	if (m_pD2DFactory) m_pD2DFactory->Release();
	if (m_pDWriteFactory) m_pDWriteFactory->Release();
	if (m_pTextFormat) m_pTextFormat->Release();
	if (m_pTextLayout) m_pTextLayout->Release();
	m_pD2DFactory = nullptr;
	m_pDWriteFactory = nullptr;
	m_pTextFormat = nullptr;
	m_pTextLayout = nullptr;
}
HRESULT	DirectWrite::CreateDeviceResources(IDXGISurface* pSurface)
{
	HRESULT hr;

	D2D1_RENDER_TARGET_PROPERTIES Property;
	//Property.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
	//D2D1_PIXEL_FORMAT PixelFormat;
	//PixelFormat.format = DXGI_FORMAT_UNKNOWN;
	//PixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
	//Property.pixelFormat = PixelFormat;
	//Property.dpiX = m_fDpiX;
	//Property.dpiY = m_fDpiY;
	//Property.usage = D2D1_RENDER_TARGET_USAGE_NONE;
	//Property.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;

	Property = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), m_fDpiX, m_fDpiY);

	hr = m_pD2DFactory->CreateDxgiSurfaceRenderTarget(pSurface, &Property, &m_pRenderTarget);
	if (SUCCEEDED(hr))
	{
		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::AliceBlue), &m_pColorBrush);
	}
	return hr;
}
void DirectWrite::DiscardDeviceResources()
{
	if (m_pRenderTarget) m_pRenderTarget->Release();
	if (m_pColorBrush) m_pColorBrush->Release();
	m_pColorBrush = nullptr;
	m_pRenderTarget = nullptr;
}
void DirectWrite::OnResize(IDXGISurface* pSurface)
{
	DiscardDeviceResources();
	CreateDeviceResources(pSurface);
}