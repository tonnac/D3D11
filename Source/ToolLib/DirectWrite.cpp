#include "DirectWrite.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX::PackedVector;
using namespace DirectX;

bool DirectWrite::Initialize()
{
	ThrowifFailed(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, m_pD2DFactory.GetAddressOf()));
	m_pD2DFactory->GetDesktopDpi(&m_fdpiX, &m_fdpiY);
	ThrowifFailed(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), RE_Casting(IUnknown**, m_pWriteFactory.GetAddressOf())));
	ThrowifFailed(m_pWriteFactory->CreateTextFormat(L"Consolas", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20, L"en-us", &m_pTextFormat));
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

void DirectWrite::DrawText(const D2D1_RECT_F& rt, const std::tstring& Text, DirectX::FXMVECTOR Color, DWRITE_TEXT_ALIGNMENT alignment)
{
	XMFLOAT4 Col;
	XMStoreFloat4(&Col, Color);
	m_pTextFormat->SetTextAlignment(alignment);
	m_pColorBrush->SetColor(D2D1::ColorF(Col.x, Col.y, Col.z, Col.w));
	m_pRenderTarget->DrawText(Text.c_str(), Casting(UINT, Text.length()), m_pTextFormat.Get(), rt, m_pColorBrush.Get());
}

void DirectWrite::OnResize(IDXGISwapChain* pSwapChain)
{
	ComPtr<IDXGISurface> pSurface = nullptr;
	pSwapChain->GetBuffer(0, IID_PPV_ARGS(pSurface.GetAddressOf()));

	D2D1_RENDER_TARGET_PROPERTIES Property;
	Property.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
	Property.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
	Property.pixelFormat.format = DXGI_FORMAT_R8G8B8A8_UNORM;
	Property.dpiX = m_fdpiX;
	Property.dpiY = m_fdpiY;
	Property.usage = D2D1_RENDER_TARGET_USAGE_NONE;
	Property.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;

	ThrowifFailed(m_pD2DFactory->CreateDxgiSurfaceRenderTarget(pSurface.Get(), &Property, m_pRenderTarget.GetAddressOf()));
	ThrowifFailed(m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::AliceBlue), m_pColorBrush.GetAddressOf()));
}

void DirectWrite::Reset()
{
	m_pRenderTarget.Reset();
	m_pColorBrush.Reset();
}